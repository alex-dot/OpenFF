#include <Corrade/Containers/Reference.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/SceneGraph/Drawable.h>

#include <fft.h>

#include <chrono>
#include <tuple>

#include <sstream>
#include <iomanip>

#include <cmath>
#include <limits>

#include "Music.h"
#include "../misc/TerminalOutput.h"

using namespace Magnum;
using namespace Math::Literals;
using namespace OpenFF;

typedef struct cfloat
{
    float real;
    float imag;
} cfloat;

Music::Music() :
        _config(nullptr),
        _context{
          Audio::Context::Configuration{}
            .setHrtf(Audio::Context::Configuration::Hrtf::Enabled)
          },
        _current_track_loaded(false),
        _source_rig(&_scene),
        _source_object(&_source_rig),
        _camera_object(&_scene),
        _camera(_camera_object),
        _listener{_scene},
        _global_pause(false),
        _visualiser_bar_count(60),
        _frame_slice_left(0),
        _frame_slice_right(0)
{
  _camera.setViewport(GL::defaultFramebuffer.viewport().size());
}
Music::Music(Configuration* config, RessourceLoader* ressource_loader) :
        Music::Music() {
  _config = config;
  _ressource_loader = ressource_loader;
  _current_track_name = _config->getRandomMusic();
  this->loadAudioData();
}
Music::Music(
        Configuration* config,
        RessourceLoader* ressource_loader,
        InputHandler* input) :
                Music::Music(config, ressource_loader) {
  this->bindCallbacks(input);
}

Music& Music::loadAudioData() {
  if(_config->getMusicLocation(_current_track_location, _current_track_name))
    _current_track_location = _config->getMusicLocation()+_current_track_location;
  else
    std::exit(2);

  _current_track_future = std::async(
          std::launch::async,
          &RessourceLoader::getAudio,
          _ressource_loader,
          _current_track_location);

  return *this;
}

Music& Music::storeAudioData() {
  auto current_track_tuple = _current_track_future.get();
  _current_track_format = std::get<0>(current_track_tuple);
  _current_track_buffer_data = std::move(std::get<1>(current_track_tuple));
  _current_track_frequency = std::get<2>(current_track_tuple);

  prepareForMagnitudeVisualiser();

  processAudioForMagnitudeVisualiser(OpenFF::MusicVisualiserChannel::left, 0, 100);
  processAudioForMagnitudeVisualiser(OpenFF::MusicVisualiserChannel::right, 0, 100);

  return *this;
}

Music& Music::playAudioData() {
  _current_track_buffer.setData(
          _current_track_format,
          _current_track_buffer_data,
          _current_track_frequency);

  (new Audio::Playable2D{_source_object, &_playables})->source()
    .setBuffer(&_current_track_buffer)
    .setLooping(true);
  if( !_global_pause ) {
    _playables.play();
  }
  _listener.setGain(0.2f);

  return *this;
}

std::tuple<unsigned int,unsigned int,float> Music::getVisualiserInformation() {
  return std::make_tuple(_visualiser_bar_count,_maximum_frame_count,_frame_window_size);
}

float OpenFF::blackmanHarrisNuttalWindowTransform(float frame, unsigned int N, unsigned int n) {
  const float a0 = 0.355768;
  const float a1 = 0.487396;
  const float a2 = 0.144232;
  const float a3 = 0.012604;
  const float pi = atan(1.0);

  const float w = a0 - a1*std::cos(2*pi*n/N) + a2*std::cos(4*pi*n/N) - a3*std::cos(6*pi*n/N);
  return frame*w;
}

Music& Music::prepareForMagnitudeVisualiser() {
  if( _current_track_format != Audio::BufferFormat::Stereo16
   && _current_track_format != Audio::BufferFormat::Mono16 ) {
     Err("Audioformats other than Stereo16 and Mono16 aren't currently supported.");
     return *this;
  }

  // First set the sampling rate for the FFT, must be n^2 and 4096 is a good default
  _samples = 4096;
  _channels = 1;

  // the _frame_window_size defines the width (in seconds) of each frame
  _frame_window_size = float(_samples/2)/float(_current_track_frequency);

  // If we have stereo, double the sampling, so each channel gets 4096 samples
  if( _current_track_format == Audio::BufferFormat::Stereo16 ) {
    _samples = 8192;
    _channels = 2;
  }

  // _maximum_frame_count defines how many frames we have given our sampling rate
  _maximum_frame_count = _current_track_buffer_data.size()/_samples;
  _magnitude_bin_matrix_left.reserve(_maximum_frame_count+1);
  _magnitude_bin_matrix_right.reserve(_maximum_frame_count+1);

  _max_magnitude = -std::numeric_limits<float>::infinity();

  // Let's define our frequency bins, notice how it is dependent on
  // _visualiser_bar_count, BUT the first 10 bins are always 20-200Hz!
  const unsigned int bass_cutoff = _visualiser_bar_count/6;
  const float max_bass_freq = 200;
  const float bass_freq_step = float(max_bass_freq)/bass_cutoff;
  const float nyquist = _current_track_frequency / 2;
  _frequency_bin.reserve(_visualiser_bar_count+2);
  for( unsigned int i = 0; i < _visualiser_bar_count+1; ++i ) {
    float freq;
    if( i < bass_cutoff ) {
      // For frequencies below 200Hz, use these custom frequencies
      freq = bass_freq_step*i;
    } else {
      // 200*exp(4.6) is ~20000Hz, so this is a nice exponential function for
      // frequencies between 200Hz and 20000Hz, relative to _visualiser_bar_count
      freq = max_bass_freq*std::exp(4.6f*(i-(bass_cutoff-1))/(_visualiser_bar_count-(bass_cutoff-1)));
    }
    _frequency_bin.push_back(freq);
  }
  _frequency_bin.push_back(nyquist);

  return *this;
}

std::vector<std::vector<float>> Music::processAudioForMagnitudeVisualiser(
        const MusicVisualiserChannel return_channel,
        const unsigned int frames_min,
        unsigned int frames_max) {

  if( frames_max <= _frame_slice_left && return_channel == left ) {
    return std::vector<std::vector<float>>(
            _magnitude_bin_matrix_left.begin()+frames_min,
            _magnitude_bin_matrix_left.begin()+frames_max);
  } else if( frames_max <= _frame_slice_right && return_channel == right ) {
    return std::vector<std::vector<float>>(
            _magnitude_bin_matrix_right.begin()+frames_min,
            _magnitude_bin_matrix_right.begin()+frames_max);
  }

  Corrade::Containers::ArrayView<char> data;

  if( frames_max >= _maximum_frame_count )
    frames_max = _maximum_frame_count;

  for( unsigned int f = frames_min; f < frames_max; ++f ) {
    if( (f+1)*_samples >= (_maximum_frame_count-1)*_samples+_samples/2 ) {
        data = _current_track_buffer_data.slice(f*_samples,f*_samples+_samples/2);
    } else {
      data = _current_track_buffer_data.slice(f*_samples,f*_samples+_samples);
    }

    unsigned int c = 0;
    unsigned int N = _samples;
    float *input = static_cast<float*>(mufft_alloc(N * sizeof(float)));
    for (unsigned int n = 0; n < N*2; n=n+c+2) {
      uint16_t data_frame_uint16;
      if ( _current_track_format == Audio::BufferFormat::Stereo16 && return_channel == right ) {
        data_frame_uint16 = static_cast<uint16_t>(
                static_cast<uint8_t>(data[n+2])) |
                  (static_cast<uint16_t>(static_cast<uint8_t>(data[n+3])) << 8);
      } else {
        data_frame_uint16 = static_cast<uint16_t>(
                static_cast<uint8_t>(data[n])) |
                  (static_cast<uint16_t>(static_cast<uint8_t>(data[n+1])) << 8);
      }
      float data_frame = static_cast<float>(
              static_cast<int16_t>(data_frame_uint16)/32768.0);  // 32768 is signed, 65535.0 otherwise
      input[n/2] = blackmanHarrisNuttalWindowTransform(data_frame, N, n/2);
    }

    cfloat *output = static_cast<cfloat*>(mufft_alloc(N * sizeof(cfloat)));
    mufft_plan_1d *muplan = mufft_create_plan_1d_r2c(N,0);
    mufft_execute_plan_1d(muplan, output, input);

    std::vector<float> magnitude_bin(
            _visualiser_bar_count+1,
            -std::numeric_limits<float>::infinity());

    for( unsigned int i = 0; i < N/2; ++i ) {
      float magnitude = std::sqrt(output[i].real*output[i].real+output[i].imag*output[i].imag);
      float frequency = float(i) * _current_track_frequency / N;

      for( unsigned int j = 0; j < magnitude_bin.size(); ++j){
        if( (frequency > _frequency_bin[j]) && (frequency <= _frequency_bin[j+1]) ) {
          if( magnitude > magnitude_bin[j] ) {
            magnitude_bin[j] = magnitude;
          }
        }
      }
      if( magnitude > _max_magnitude )
        _max_magnitude = magnitude;
    }

    if ( return_channel == left ) {
      _magnitude_bin_matrix_left.push_back(magnitude_bin);
    } else {
      _magnitude_bin_matrix_right.push_back(magnitude_bin);
    }

    mufft_free(input);
    mufft_free(output);
    mufft_free_plan_1d(muplan);
  }

  if ( return_channel == left ) {
    for( auto i=_magnitude_bin_matrix_left.begin()+frames_min;
            i<_magnitude_bin_matrix_left.begin()+frames_max; ++i ) {
      unsigned int freq = 0;
      for( auto j = i->begin(); j < i->end(); ++j ) {
        *j = Math::min(*j/_max_magnitude * Math::max(10*_frequency_bin[freq]/_frequency_bin.back(),1.0f),0.8f);
        freq = freq+1;
      }
    }
  } else {
    for( auto i=_magnitude_bin_matrix_right.begin()+frames_min;
            i<_magnitude_bin_matrix_right.begin()+frames_max; ++i ) {
      unsigned int freq = 0;
      for( auto j = i->begin(); j < i->end(); ++j ) {
        *j = Math::min(*j/_max_magnitude * Math::max(10*_frequency_bin[freq]/_frequency_bin.back(),1.0f),0.8f);
          freq = freq+1;
      }
    }
  }

  if( return_channel == left ) {
    _frame_slice_left = frames_max;
    return std::vector<std::vector<float>>(
            _magnitude_bin_matrix_left.begin()+frames_min,
            _magnitude_bin_matrix_left.begin()+frames_max);
  } else {
    _frame_slice_right = frames_max;
    return std::vector<std::vector<float>>(
            _magnitude_bin_matrix_right.begin()+frames_min,
            _magnitude_bin_matrix_right.begin()+frames_max);
  }
}

std::string Music::getCurrentTrackName() {
  std::string name;
  _config->getMusicName(name, _current_track_name);
  return name;
}

bool Music::isPaused() {
  return (_global_pause || !_current_track_loaded);
}

Music& Music::increaseGain() {
  _listener.setGain(_listener.gain()+0.1f);
  this->draw();
  return *this;
}
Music& Music::decreaseGain() {
  _listener.setGain(_listener.gain()-0.1f);
  this->draw();
  return *this;
}
Music& Music::pauseResume() {
  if( _global_pause ) {
    _playables.play();
    _global_pause = false;
  } else {
    _playables.pause();
    _global_pause = true;
  }
  return *this;
}

void Music::draw() {
  if( _current_track_loaded ) {
    _listener.update({_playables});
    _camera.draw(_drawables);
  } else {
    if( _current_track_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
      this->storeAudioData();
      this->playAudioData();
      _current_track_loaded = true;
    }
  }
}

void Music::bindCallbacks(InputHandler* input) {
  input->setCallbacks(
      *this,
      ObjectType::music,
      {
          std::make_pair(&Music::increaseGain,InputEvents::music_increase_gain),
          std::make_pair(&Music::decreaseGain,InputEvents::music_decrease_gain),
          std::make_pair(&Music::pauseResume,InputEvents::music_pause)
      }
  );
}
