#include "MusicMenu.h"
#include "../misc/TerminalOutput.h"

#include <list>
#include <algorithm>

using namespace OpenFF;

MusicMenu::MusicMenu(
        Configuration* config,
        RessourceLoader* ressource_loader,
        InputHandler* input_handler,
        Vector2 relative_billboard_ratio) :
                AbstractMenu::AbstractMenu(),
                _config(config),
                _stopped_the_music(2),
                _visualiser_loaded(0),
                _visualiser_preparing(false),
                _focus_big(true),
                _current_time(0.0f),
                _magnitude_bin_matrix_left_fully_loaded(false),
                _magnitude_bin_matrix_right_fully_loaded(false) {

  // load music object
  _music = new OpenFF::Music(config, ressource_loader, input_handler);
  _music->setTrackByName("ta");

  // set Title textbox
  _songtitle = new OpenFF::Textbox(
          config,
          ressource_loader,
          relative_billboard_ratio,
          Vector2i(314,24),
          Vector2i(3,200));
  setTitle(_music->getCurrentTrackName());

  // set Player textbox
  _player = new OpenFF::MenuBox(
          MenuBoxType::freeform,
          config,
          ressource_loader,
          relative_billboard_ratio,
          Vector2i(68,24),
          Vector2i(250,10));
  _player->enableInstantRendering();
  _player->write("«■▶Ⅱ»");
  _player->moveCharacter(0,0,Vector2i(1,0));
  _player->moveCharacter(0,3,Vector2i(1,1));
  _player->moveCharacter(0,4,Vector2i(1,0));
  _player->setSelectionAccept(
          Vector2i(0,0).flipped(),
          this, &MusicMenu::previousTrack);
  _player->setSelectionAccept(
          Vector2i(0,1).flipped(),
          this, &MusicMenu::stopTrack);
  _player->setSelectionAccept(
          Vector2i(0,2).flipped(),
          this, &MusicMenu::resumeTrack);
  _player->setSelectionAccept(
          Vector2i(0,3).flipped(),
          this, &MusicMenu::pauseTrack);
  _player->setSelectionAccept(
          Vector2i(0,4).flipped(),
          this, &MusicMenu::nextTrack);

  _unsorted_menu_boxes.push_back(_player);
  _active_box = &(_player->enableSelection());

  this->sortMenuboxes();
  _active_box_location = findMenubox(_active_box);

  _timeline.start();
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
MusicMenu& MusicMenu::pauseResumeTrack() {
  _music->pauseResume();
  return *this;
}
MusicMenu& MusicMenu::pauseTrack() {
  _music->pause();
  return *this;
}
MusicMenu& MusicMenu::resumeTrack() {
  _music->resume();
  return *this;
}
MusicMenu& MusicMenu::previousTrack() {
  _music->pauseResume();
  return *this;
}
MusicMenu& MusicMenu::nextTrack() {
  stopTrack();
  // get a new random track; to prevent the same song playing twice,
  // small while-loop to select the next song if the oracle gave the same
  std::string random_track;
  std::string random_track_name_long = _music->getCurrentTrackName();
  while( _music->getCurrentTrackName() == random_track_name_long ) {
    random_track = _config->getRandomMusic();
    _config->getMusicName(random_track_name_long, random_track);
  }
  _music->setTrackByName(random_track);
  _songtitle->write(_music->getCurrentTrackName());
  return *this;
}
MusicMenu& MusicMenu::stopTrack() {
  _music->stop();
  _music->enableMusicPendingFutures();
  _stopped_the_music = 0;
  return *this;
}

void MusicMenu::draw() {
  _current_time = _current_time + _timeline.previousFrameDuration();

  // If the music isn't playing, don't play the visualiser
  if( !_music->isPaused() ) {
    // If the visualiser isn't fully loaded or is reloading, load the visualiser
    if( _visualiser_loaded < 2 || _stopped_the_music < 2 ) {
      // If the visualiser isn't (re-)initilised, prepare the visualiser
      if( !_visualiser_preparing || _stopped_the_music < 2 ) {
        prepareVisualiser();
      } else {
        magnitudeSlicer();
      }
    } else {
      drawVisualiser();
    }
  } else {
    // If there are still futures dangling, let them finish, after signal the music object
    if( _magnitude_bin_matrix_left_future.valid() || _magnitude_bin_matrix_right_future.valid() ) {
      if( _magnitude_bin_matrix_left_future.valid()
       && _magnitude_bin_matrix_left_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
        auto temp_matrix = _magnitude_bin_matrix_left_future.get();
      }
      if( _magnitude_bin_matrix_right_future.valid()
       && _magnitude_bin_matrix_right_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
        auto temp_matrix = _magnitude_bin_matrix_right_future.get();
      }
    } else {
      _music->disableMusicPendingFutures();
    }
  }

  _songtitle->draw();
  _player->draw(_current_time);

  for( auto i = _unsorted_menu_boxes.begin(); i < _unsorted_menu_boxes.end(); ++i) {
    (*i)->draw(_current_time);
  }

  _music->draw();

  _timeline.nextFrame();
}

void MusicMenu::magnitudeSlicer() {
  if( !_magnitude_bin_matrix_left_fully_loaded ) {
    if( !_magnitude_bin_matrix_left_future.valid() ) {
      _magnitude_bin_matrix_left_future = std::async(
              std::launch::async,
              &Music::processAudioForMagnitudeVisualiser,
              _music,
              OpenFF::MusicVisualiserChannel::channel_left,
              _frame_slice_left,
              _frame_slice_left+_frame_slice_window);
    } else {
      if( _magnitude_bin_matrix_left_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
        auto temp_matrix = _magnitude_bin_matrix_left_future.get();
        if( _stopped_the_music < 2 ) {
          _stopped_the_music++;
        } else {
          _magnitude_bin_matrix_left.insert(_magnitude_bin_matrix_left.begin()+_frame_slice_left,temp_matrix.begin(),temp_matrix.end());
          _frame_slice_left = _frame_slice_left + _frame_slice_window;
          if( _frame_slice_left > _maximum_frame_count )
            _magnitude_bin_matrix_left_fully_loaded = true;
          if( _visualiser_loaded < 2 )
            _visualiser_loaded++;
        }
      }
    }
  }
  if( !_magnitude_bin_matrix_right_fully_loaded ) {
    if( !_magnitude_bin_matrix_right_future.valid() ) {
      _magnitude_bin_matrix_right_future = std::async(
              std::launch::async,
              &Music::processAudioForMagnitudeVisualiser,
              _music,
              OpenFF::MusicVisualiserChannel::channel_right,
              _frame_slice_right,
              _frame_slice_right+_frame_slice_window);
    } else {
      if( _magnitude_bin_matrix_right_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
        auto temp_matrix = _magnitude_bin_matrix_right_future.get();
        if( _stopped_the_music < 2 ) {
          _stopped_the_music++;
        } else {
          _magnitude_bin_matrix_right.insert(_magnitude_bin_matrix_right.begin()+_frame_slice_right,temp_matrix.begin(),temp_matrix.end());
          _frame_slice_right = _frame_slice_right + _frame_slice_window;
          if( _frame_slice_right > _maximum_frame_count )
            _magnitude_bin_matrix_right_fully_loaded = true;
          if( _visualiser_loaded < 2 )
            _visualiser_loaded++;
        }
      }
    }
  }
}

MusicMenu& MusicMenu::drawVisualiser() {
  unsigned int frame = static_cast<unsigned int>(
          std::floor(_current_time/_frame_window_size));
  if( frame == _maximum_frame_count ) {
    frame = 0;
    _timeline.stop();
    _timeline.start();
    _current_time = 0.0f;
  }

  magnitudeSlicer();

  _visualiser->draw(
          _magnitude_bin_matrix_left[frame],
          _magnitude_bin_matrix_right[frame]);

  return *this;
}

MusicMenu& MusicMenu::prepareVisualiser() {
  _visualiser_loaded = 0;
  _magnitude_bin_matrix_left_fully_loaded = false;
  _magnitude_bin_matrix_right_fully_loaded = false;
  _visualiser_preparing = true;
  _current_time = 0.0f;

  std::tuple<unsigned int,unsigned int,float>
          visualiser_info = _music->getVisualiserInformation();
  _visualiser_bar_count = std::get<0>(visualiser_info);
  _maximum_frame_count = std::get<1>(visualiser_info);
  _frame_window_size = std::get<2>(visualiser_info);
  _frame_slice_window = 100;
  _frame_slice_left = 0;
  _frame_slice_right = 0;
  _visualiser = new OpenFF::MusicVisualiser(_visualiser_bar_count);

  _magnitude_bin_matrix_left.clear();
  _magnitude_bin_matrix_right.clear();
  _magnitude_bin_matrix_left.reserve(_maximum_frame_count);
  _magnitude_bin_matrix_right.reserve(_maximum_frame_count);

  magnitudeSlicer();

  return *this;
}

void MusicMenu::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _songtitle->setRelativeBillboardRatio(relative_billboard_ratio);
  _player->setRelativeBillboardRatio(relative_billboard_ratio);
}

void MusicMenu::bindCallbacks(InputHandler* input) {
  bindCallbacksAbstract(input);
  input->setCallbacks(
      *this,
      ObjectType::menu_music,
      {
          std::make_pair(&MusicMenu::increaseGain,InputEvents::music_increase_gain),
          std::make_pair(&MusicMenu::decreaseGain,InputEvents::music_decrease_gain),
          std::make_pair(&MusicMenu::pauseResumeTrack,InputEvents::music_pause),
      }
  );
}
