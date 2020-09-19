#include "MusicMenu.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

MusicMenu::MusicMenu(
        Configuration* config,
        RessourceLoader* ressource_loader,
        InputHandler* input_handler,
        Vector2 relative_billboard_ratio) :
                _visualiser_loaded(false),
                _current_time(0.0f),
                _magnitude_bin_matrix_left_fully_loaded(false),
                _magnitude_bin_matrix_right_fully_loaded(false) {

  // load music object
  _music = new OpenFF::Music(config, ressource_loader, input_handler);

  // set Title textbox
  _songtitle = new OpenFF::Textbox(
          config,
          ressource_loader,
          relative_billboard_ratio,
          Vector2i(314,24),
          Vector2i(3,200));
  setTitle(_music->getCurrentTrackName());

  // set Player textbox
  _player = new OpenFF::Textbox(
          config,
          ressource_loader,
          relative_billboard_ratio,
          Vector2i(68,24),
          Vector2i(250,10));
  _player->write("« ■ ▶ II »");
  _player->show();
}

void MusicMenu::setTitle(std::string title) {
  _songtitle->write(title);
  _songtitle->show();
}

void MusicMenu::setMusic(Music* music) {
  delete(_music);
  _music = music;
}

MusicMenu& MusicMenu::increaseGain() {
  _music->increaseGain();
  return *this;
}
MusicMenu& MusicMenu::decreaseGain() {
  _music->decreaseGain();
  return *this;
}
MusicMenu& MusicMenu::pauseResume() {
  _music->pauseResume();
  return *this;
}

void MusicMenu::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _songtitle->setRelativeBillboardRatio(relative_billboard_ratio);
}

void MusicMenu::draw() {
  if( !_music->isPaused() ) {
    if( !_visualiser_loaded )
      prepareVisualiser();
    drawVisualiser();
  }

  _songtitle->draw();
  _player->draw();

  _music->draw();

  _timeline.nextFrame();
}

MusicMenu& MusicMenu::drawVisualiser() {
  _current_time = _current_time + _timeline.previousFrameDuration();
  unsigned int frame = static_cast<unsigned int>(
          std::floor(_current_time/_frame_window_size));
  if( frame == _maximum_frame_count ) {
    frame = 0;
    _timeline.stop();
    _timeline.start();
    _current_time = 0.0f;
  }

  if( !_magnitude_bin_matrix_left_fully_loaded ) {
    if( !_magnitude_bin_matrix_left_future.valid() ) {
      _magnitude_bin_matrix_left_future = std::async(
              std::launch::async,
              &Music::processAudioForMagnitudeVisualiser,
              _music,
              OpenFF::MusicVisualiserChannel::left,
              _frame_slice_left,
              _frame_slice_left+_frame_slice_window);
    } else {
      if( _magnitude_bin_matrix_left_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
        auto temp_matrix = _magnitude_bin_matrix_left_future.get();
        _magnitude_bin_matrix_left.insert(_magnitude_bin_matrix_left.end(),temp_matrix.begin(),temp_matrix.end());
        _frame_slice_left = _frame_slice_left + _frame_slice_window;
      if( _frame_slice_left > _maximum_frame_count )
        _magnitude_bin_matrix_left_fully_loaded = true;
      }
    }
  }
  if( !_magnitude_bin_matrix_right_fully_loaded ) {
    if( !_magnitude_bin_matrix_right_future.valid() ) {
      _magnitude_bin_matrix_right_future = std::async(
              std::launch::async,
              &Music::processAudioForMagnitudeVisualiser,
              _music,
              OpenFF::MusicVisualiserChannel::right,
              _frame_slice_right,
              _frame_slice_right+_frame_slice_window);
    } else {
      if( _magnitude_bin_matrix_right_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
        auto temp_matrix = _magnitude_bin_matrix_right_future.get();
        _magnitude_bin_matrix_right.insert(_magnitude_bin_matrix_right.begin()+_frame_slice_right,temp_matrix.begin(),temp_matrix.end());
        _frame_slice_right = _frame_slice_right + _frame_slice_window;
        if( _frame_slice_right > _maximum_frame_count )
          _magnitude_bin_matrix_right_fully_loaded = true;
      }
    }
  }

  _visualiser->draw(
          _magnitude_bin_matrix_left[frame],
          _magnitude_bin_matrix_right[frame]);

  return *this;
}

MusicMenu& MusicMenu::prepareVisualiser() {
  _timeline.start();

  std::tuple<unsigned int,unsigned int,float>
          visualiser_info = _music->getVisualiserInformation();
  _visualiser_bar_count = std::get<0>(visualiser_info);
  _maximum_frame_count = std::get<1>(visualiser_info);
  _frame_window_size = std::get<2>(visualiser_info);
  _frame_slice_window = 100;
  _frame_slice_left = 0;
  _frame_slice_right = 0;
  _visualiser = new OpenFF::MusicVisualiser(_visualiser_bar_count);

  _magnitude_bin_matrix_left.reserve(_maximum_frame_count);
  _magnitude_bin_matrix_right.reserve(_maximum_frame_count);

  std::vector<std::vector<float>> temp_matrix;
  temp_matrix = _music->processAudioForMagnitudeVisualiser(OpenFF::MusicVisualiserChannel::left, _frame_slice_left, _frame_slice_window);
  _magnitude_bin_matrix_left.insert(_magnitude_bin_matrix_left.begin(),temp_matrix.begin(),temp_matrix.end());
  temp_matrix = _music->processAudioForMagnitudeVisualiser(OpenFF::MusicVisualiserChannel::right, _frame_slice_right, _frame_slice_window);
  _magnitude_bin_matrix_right.insert(_magnitude_bin_matrix_right.begin(),temp_matrix.begin(),temp_matrix.end());

  _frame_slice_left = _frame_slice_left + _frame_slice_window;
  _frame_slice_right = _frame_slice_right + _frame_slice_window;
  _visualiser_loaded = true;

  return *this;
}

void MusicMenu::bindCallbacks(InputHandler* input) {
  input->setCallbacks(
      *this,
      ObjectType::menu_music,
      {
          std::make_pair(&MusicMenu::increaseGain,InputEvents::music_increase_gain),
          std::make_pair(&MusicMenu::decreaseGain,InputEvents::music_decrease_gain),
          std::make_pair(&MusicMenu::pauseResume,InputEvents::music_pause)
      }
  );
}
