#include "MusicMenu.h"
#include "../misc/TerminalOutput.h"

#include <list>
#include <algorithm>

// for testing and posterity
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace OpenFF;

MusicMenu::MusicMenu(
        Configuration* config,
        RessourceLoader* ressource_loader,
        InputHandler* input_handler,
        Vector2 relative_billboard_ratio) :
                _visualiser_loaded(false),
                _visualiser_preparing(false),
                _focus_big(true),
                _current_time(0.0f),
                _magnitude_bin_matrix_left_fully_loaded(false),
                _magnitude_bin_matrix_right_fully_loaded(false) {
/*
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
  _player = new OpenFF::MenuBox(
          MenuBoxType::freeform,
          config,
          ressource_loader,
          relative_billboard_ratio,
          Vector2i(68,24),
          Vector2i(250,10));
  _player->write("«■▶Ⅱ»");
  _player->moveCharacter(0,0,Vector2i(1,0));
  _player->moveCharacter(0,3,Vector2i(1,1));
  _player->moveCharacter(0,4,Vector2i(1,0));
*/
  OpenFF::MenuBox* box = new OpenFF::MenuBox(
    config, ressource_loader, relative_billboard_ratio,
    Vector2i(80,24), Vector2i(140,96));
  box->write("create");
//  box->write("Box #"+std::to_string(1));
  box->setSelectionAccept(Vector2i(0),this,&MusicMenu::createNewMenubox);
  _unsorted_menu_boxes.push_back(box);
  _active_box = &(box->enableSelection());
/*
  box = new OpenFF::MenuBox(
    MenuBoxType::freeform, config, ressource_loader, relative_billboard_ratio,
    Vector2i(80,36), Vector2i(40,60));
  box->write("Box #"+std::to_string(2)+"\nblub");
  _unsorted_menu_boxes.push_back(box);
  box = new OpenFF::MenuBox(
    config, ressource_loader, relative_billboard_ratio,
    Vector2i(120,24), Vector2i(125,35));
  box->write("Box #"+std::to_string(3));
  _unsorted_menu_boxes.push_back(box);
  box = new OpenFF::MenuBox(
    config, ressource_loader, relative_billboard_ratio,
    Vector2i(110,140), Vector2i(250,20));
  box->write("Box #"+std::to_string(4)
        +"\n2nd line\n3rd\n4th line\n5th line\n6th line\n7th line\n8th line\n9th line");
  _unsorted_menu_boxes.push_back(box);
  box->setLinkedBoxUp(_active_box).setLinkedBoxDown(_active_box);
  box = new OpenFF::MenuBox(
    config, ressource_loader, relative_billboard_ratio,
    Vector2i(80,24), Vector2i(165,100));
  box->write("Box #"+std::to_string(5));
  _unsorted_menu_boxes.push_back(box);
*/

  this->sortMenuboxes();
  _active_box_location = findMenubox(_active_box);

  // for testing and posterity
  srand (time(NULL));
  _config = config;
  _ressource_loader = ressource_loader;
  _relative_billboard_ratio = relative_billboard_ratio;

  _timeline.start();
}

void MusicMenu::createNewMenubox() {
  int x = (rand() % 6 + 1 ) * 40;
  int y = (rand() % 12 + 1 ) * 20;
  int t = rand() % 2;
  Dbg{} << "oracle: " << x << y << t;

  OpenFF::MenuBox* box = new OpenFF::MenuBox(
    _config, _ressource_loader, _relative_billboard_ratio,
    Vector2i(80,24), Vector2i(x,y));

  if( t==0 ) {
    box->write("create");
    box->setSelectionAccept(Vector2i(0),this,&MusicMenu::createNewMenubox);
  } else {
    box->write("delete");
    box->setSelectionAccept(Vector2i(0),this,&MusicMenu::deleteOneMenubox);
  }

  _unsorted_menu_boxes.push_back(box);
  this->sortMenuboxes();
}
void MusicMenu::deleteOneMenubox() {
  int x = rand() % _unsorted_menu_boxes.size();
  auto i = _unsorted_menu_boxes.begin()+x;
  _unsorted_menu_boxes.erase(i);
  this->sortMenuboxes();
}

void MusicMenu::setTitle(std::string title) {
  _songtitle->write(title);
  _songtitle->show();
}

void MusicMenu::setMusic(Music* music) {
  delete(_music);
  _music = music;
}

MusicMenu& MusicMenu::menuUp() {
  this->moveSelection(MenuDirections::up);
  return *this;
}
MusicMenu& MusicMenu::menuDown() {
  this->moveSelection(MenuDirections::down);
  return *this;
}
MusicMenu& MusicMenu::menuLeft() {
  this->moveSelection(MenuDirections::left);
  return *this;
}
MusicMenu& MusicMenu::menuRight() {
  this->moveSelection(MenuDirections::right);
  return *this;
}
MusicMenu& MusicMenu::moveSelection(MenuDirections dir) {
  MenuSelectionReturns ret = MenuSelectionReturns::error;
  switch(dir) {
    case(MenuDirections::up):
            ret = _active_box->selectionUp();
            break;
    case(MenuDirections::down):
            ret = _active_box->selectionDown();
            break;
    case(MenuDirections::left):
            ret = _active_box->selectionLeft();
            break;
    case(MenuDirections::right):
            ret = _active_box->selectionRight();
            break;
    default:
            ret = MenuSelectionReturns::error;
            break;
  }

  switch(ret) {
    case(MenuSelectionReturns::success):
            break;
    case(MenuSelectionReturns::end_of_line): {
            MenuBox* new_active_box = this->getNextMenubox(dir);
            if( new_active_box ) {
              _active_box->disableSelection();
              _active_box = new_active_box;
              _active_box->enableSelection();
            }
            break;
        }
    case(MenuSelectionReturns::error):
    default:
            FatlSrcLogicError("Could not move selection. Direction was: "+std::to_string(dir));
            break;
  }
  return *this;
}

MusicMenu& MusicMenu::menuAcceptSelected() {
  _active_box->selectionAccept();
  return *this;
}

// Called when menu is closed
MusicMenu& MusicMenu::menuExit() {
  return *this;
}

// Menu Box Sorting
// We are using a 2d menu layout array. Small changes of the boxes size or
// location shall lead to (sometimes radical) changes in the menu's layout.
//********************************************************************************
//  _   #1    _            *   _   #2    _            *   _   #3    _            *
// | |  ___  | |     x  x  *  |_|  ___  | |     x  x  *  |_|       | |     x  x  *
// |_| |___| | |  => xxxx  *   _  |___| | |  =>  xxx  *   _   ___  | |  =>    x  *
// |_|    _  | |     x  x  *  |_|    _  | |     x  x  *  |_| |___| | |     xxxx  *
//       |_| |_|       xx  *        |_| |_|       xx  *        |_| |_|       xx  *
//                         *                          *                          *
//********************************************************************************
// The 2d menu layout array shall also be used as a consistent way to predict
// to which next box the user's selection goes when it goes beyond the box's
// limits.
//********************************************  Always go horizontally or
//  _    #1       _        *   _   #1a   _   *  verically, unless there is a box
// | |\   ___   /| |       *  |_|  ___  | |  *  diagonally in the same direction
// |_|<->|___|<->| |       *   _  |___| | |  *  one space away. If there is, use
// |_|/   \ ^   \| |       *  |_|    _  |?|  *  that box instead. I.e. moving
//         \|   /| |       *        |_| |_|  *  right in the box top-left would
//         |_|<->|_|       *                 *  place the selection in case #1
//                         *                 *  in the middle box, but in the
//********************************************  far-right box in case #3.
// If a box has no direct horizontal or vertical neighbor and two different
// boxes in both diagonal directions (case #1a), use the more salient one,
// which is by default the size-wise larger one.
MusicMenu& MusicMenu::sortMenuboxes() {
  std::list<int> rows_list;
  std::list<int> cols_list;

  int row_divisor = default_viewport.x() / 20;
  int col_divisor = default_viewport.y() / 20;

  for( auto i = _unsorted_menu_boxes.begin(); i != _unsorted_menu_boxes.end(); ++i ) {
    rows_list.push_back((*i)->getOffset().y() / row_divisor);
    cols_list.push_back((*i)->getOffset().x() / col_divisor);
  }

  rows_list.sort();
  rows_list.unique();
  rows_list.resize(rows_list.size());
  cols_list.sort();
  cols_list.unique();
  cols_list.resize(cols_list.size());

  std::vector<int> rows(rows_list.begin(),rows_list.end());
  std::vector<int> cols(cols_list.begin(),cols_list.end());

  MenuBoxElement new_mbe;
  std::vector<std::vector<MenuBoxElement>> mb_col(cols.size(),std::vector<MenuBoxElement>(1,new_mbe));
  std::vector<std::vector<std::vector<MenuBoxElement>>> mb_row(rows.size(),mb_col);

  for( unsigned int i = 0; i < mb_row.size(); ++i ) {
    for( unsigned int j = 0; j < mb_row[i].size(); ++j ) {
      mb_row[i][j][0].rowid = rows[i];
      mb_row[i][j][0].colid = cols[j];
    }
  }

  for( auto i = mb_row.begin(); i != mb_row.end(); ++i ) {
    for( auto j = i->begin(); j != i->end(); ++j ) {
      for( auto k = _unsorted_menu_boxes.begin(); k != _unsorted_menu_boxes.end(); ++k ) {
        // We check for three possibilities: (a) the top-left corner fits exactly
        // into the 2D-array; (b) the top-left corner fits into the row while the
        // top-right corner is greater than the column; or (c) the top-left corner
        // fits into the column while the lower-left corner is greather than the
        // current row.
        if( ( (*k)->getOffset().y() / row_divisor == j->front().rowid
           && (*k)->getOffset().x() / col_divisor == j->front().colid )
         || ( (*k)->getOffset().x() / col_divisor == j->front().colid
           && (*k)->getOffset().y() / row_divisor <  j->front().rowid
           && ((*k)->getTextboxSize().y() + (*k)->getOffset().y()) / row_divisor > j->front().rowid )
         || ( (*k)->getOffset().y() / row_divisor == j->front().rowid
           && (*k)->getOffset().x() / col_divisor <  j->front().colid
           && ((*k)->getTextboxSize().x() + (*k)->getOffset().x()) / col_divisor > j->front().colid ) ) {
          MenuBoxElement mbe;
          mbe.weight = 1;
          mbe.rowid = j->front().rowid;
          mbe.colid = j->front().colid;
          mbe.col_pixels = (j->front().colid+1)*col_divisor;
          mbe.row_pixels = (j->front().rowid+1)*row_divisor;
          mbe.ptr = *k;
          if( !j->front().weight )
            j->erase(j->begin());
          j->push_back(mbe);
        }
      }
    }
  }

  std::swap(_menu_boxes, mb_row);

  Dbg{} << "matrix:";
  for( auto i = 0; i < _menu_boxes.size(); ++i ) {
    std::string row = "";
    for( auto j = 0; j < _menu_boxes[i].size(); ++j ) {
      if( _menu_boxes[i][j][0].ptr != nullptr ) {
        row += "1";
      } else {
        row += "0";
      }
    }
    Dbg{} << row;
  }
  Dbg{} << "";

  for( auto k = _unsorted_menu_boxes.begin(); k != _unsorted_menu_boxes.end(); ++k ) {
    std::vector<Vector2i> menuboxes = findAllMenuboxListLocations(*k);
    if( menuboxes.size() > 1 ) {
      Vector2i last_list_location = menuboxes.front();
      std::vector<Vector2i> instances;
      instances.push_back(last_list_location);
      for( auto m = menuboxes.begin()+1; m < menuboxes.end(); ++m ) {
        if( last_list_location.x() < m->x() ) {
          instances.push_back(last_list_location+Vector2i(1,0));
        }
        if( last_list_location.y() < m->y() ) {
          instances.push_back(last_list_location+Vector2i(0,1));
        }
        last_list_location = instances.back();
      }

      (*k)->setInstancesInMenuList(instances);
    }
  }

  return *this;
}
Vector2i MusicMenu::findMenubox(MenuBox* menubox) {
  for( unsigned int i = 0; i < _menu_boxes.size(); ++i ) {
    for( unsigned int j = 0; j < _menu_boxes[i].size(); ++j ) {
      for( auto k = _menu_boxes[i][j].begin(); k != _menu_boxes[i][j].end(); ++k ) {
        if( k->ptr == menubox ) {
          return Vector2i(j,i);
        }
      }
    }
  }
  return Vector2i(-1);
}
std::vector<Vector2i> MusicMenu::findAllMenuboxListLocations(MenuBox* menubox) {
  std::vector<Vector2i> vec;
  for( unsigned int i = 0; i < _menu_boxes.size(); ++i ) {
    for( unsigned int j = 0; j < _menu_boxes[i].size(); ++j ) {
      for( auto k = _menu_boxes[i][j].begin(); k != _menu_boxes[i][j].end(); ++k ) {
        if( k->ptr == menubox ) {
          vec.push_back(Vector2i(j,i));
        }
      }
    }
  }
  return vec;
}
void MusicMenu::getMenuboxElement(MenuBoxElement& mb, Vector2i unsafe_location) {
  unsigned int x = unsafe_location.x();
  unsigned int y = unsafe_location.y();
  if( unsafe_location.y() >= static_cast<int>(_menu_boxes.size()) ) {
    y = y % _menu_boxes.size();
  } else if( unsafe_location.y() < 0 ) {
    y = _menu_boxes.size() + y;
  }
  if( unsafe_location.x() >= static_cast<int>(_menu_boxes[0].size()) ) {
    x = x % _menu_boxes[0].size();
  } else if( unsafe_location.x() < 0 ) {
    x = _menu_boxes[0].size() + x;
  }

  mb = _menu_boxes[y][x].front();
}
MenuBox* MusicMenu::getMenubox(Vector2i unsafe_location) {
  MenuBoxElement mbe;
  this->getMenuboxElement(mbe, unsafe_location);
  return mbe.ptr;
}

MenuBox* MusicMenu::getNextMenubox(MenuDirections dir) {
  Vector2i location = _active_box_location;

  if( !_active_box->isSingleInstanceInMenuLust() ) {
    switch(dir) {
      case(MenuDirections::up):
      case(MenuDirections::down):
        if( _active_box->getType() == MenuBoxType::freeform ) {
          std::vector<Vector2i> instances = _active_box->getInstancesInMenuList();
          for( auto i = instances.end()-1; i >= instances.begin(); --i ) {
            MenuBoxElement mbe;
            getMenuboxElement(mbe, Vector2i(i->x(),i->y()));
            if( _active_box->getSelectionOffset().x() >= mbe.col_pixels ) {
              location = *i;
              break;
            }
          }
        }
        break;
      case(MenuDirections::left):
      case(MenuDirections::right):
        {
          std::vector<Vector2i> instances = _active_box->getInstancesInMenuList();
          for( auto i = instances.end()-1; i >= instances.begin(); --i ) {
            MenuBoxElement mbe;
            getMenuboxElement(mbe, Vector2i(i->x(),i->y()));
            if( _active_box->getSelectionOffset().y() >= mbe.row_pixels ) {
              location = *i;
              break;
            }
          }
        }
        break;
      default:
        break;
    }
  }

  MenuBox* mb = getNextMenubox(dir, location);

  return mb;
}
MenuBox* MusicMenu::getNextMenubox(MenuDirections dir, Vector2i location) {
  MenuBoxElement menubox;
  MenuBoxElement interdimensional_menubox;
  MenuBox* linked_box = _active_box->getLinkedBox(dir);
  if( linked_box && findMenubox(linked_box) != Vector2i(-1) ) {
    getMenuboxElement(menubox, findMenubox(linked_box));
    if( _active_box == linked_box ) {
      switch(dir) {
        case(MenuDirections::up):
          _active_box->setSelectionBottom();
          break;
        case(MenuDirections::down):
          _active_box->setSelectionTop();
          break;
        case(MenuDirections::left):
          _active_box->setSelectionRightmost();
          break;
        case(MenuDirections::right):
          _active_box->setSelectionLeftmost();
          break;
        default:
          break;
      }
    }
  } else {
    switch(dir) {
      case(MenuDirections::up):
      case(MenuDirections::down):
        for( unsigned int i = 1; i < _menu_boxes[0].size(); ++i ) {
          nearestNeighbourMenubox(menubox, interdimensional_menubox, location, i, dir);
          if( menubox.ptr ) {
            _active_box_location = findMenubox(menubox.ptr);
            break;
          }
        }
        break;
      case(MenuDirections::left):
      case(MenuDirections::right):
        for( unsigned int i = 1; i < _menu_boxes.size(); ++i ) {
          nearestNeighbourMenubox(menubox, interdimensional_menubox, location, i, dir);
          if( menubox.ptr ) {
            _active_box_location = findMenubox(menubox.ptr);
            break;
          }
        }
        break;
      default:
        break;
    }
  }

  if( menubox.ptr ) {
    return menubox.ptr;
  } else if( interdimensional_menubox.ptr ) {
    _active_box_location = findMenubox(interdimensional_menubox.ptr);
    return interdimensional_menubox.ptr;
  }
  return nullptr;
}

// This is the lookup method corresponding to the Menu Box Sorting algorithm.
// Note that this is a best guess approach that will fail on edge cases.
// Consider using linked boxes instead.
void MusicMenu::nearestNeighbourMenubox(
        MenuBoxElement& mb,
        MenuBoxElement& idmb,
        Vector2i location,
        unsigned int depth,
        MenuDirections dir) {
  unsigned int d;
  unsigned int j;
  switch(dir) {
    case(MenuDirections::up):
      d = ( depth < _menu_boxes.size() ) ? depth : _menu_boxes.size()-1;
      j = -1;
      break;
    case(MenuDirections::down):
      d = ( depth < _menu_boxes.size() ) ? depth : _menu_boxes.size()-1;
      j = 1;
      break;
    case(MenuDirections::left):
      d = ( depth < _menu_boxes[0].size() ) ? depth : _menu_boxes[0].size()-1;
      j = -1;
      break;
    case(MenuDirections::right):
      d = ( depth < _menu_boxes[0].size() ) ? depth : _menu_boxes[0].size()-1;
      j = 1;
      break;
    default:
      break;
  }

  for( unsigned int i = d; i > 0; --i ) {

    int dx;
    int dy;
    if( dir == MenuDirections::left || dir == MenuDirections::right ) {
      dx = i;
      dy = d - i;
    } else if( dir == MenuDirections::up || dir == MenuDirections::down ) {
      dy = i;
      dx = d - i;
    }

    // Let's try the canonical direction first
    if( dir == MenuDirections::left || dir == MenuDirections::right ) {
      if( (int(_active_box_location.x() + dx*j) < 0)
           || (_active_box_location.x() + dx*j >= _menu_boxes[0].size()) ) {
        if( !idmb.ptr )
          getMenuboxElement(idmb, location + Vector2i(dx*j, dy*(-1)));
      } else {
        if( (int(_active_box_location.y() + dy*(-1)) < 0)
             || (_active_box_location.y() + dy*(-1) >= int(_menu_boxes.size())) ) {
          getMenuboxElement(idmb, location + Vector2i(dx*j, dy*(-1)));
        } else {
          getMenuboxElement(mb, location + Vector2i(dx*j, dy*(-1)));
        }
      }
    } else {
      if( (int(_active_box_location.y() + dy*j) < 0)
           || (_active_box_location.y() + dy*j >= _menu_boxes.size()) ) {
        if( !idmb.ptr )
          getMenuboxElement(idmb, location + Vector2i(dx, dy*j));
      } else {
        if( (int(_active_box_location.x() + dx) < 0)
             || ((_active_box_location.x() + dx) >= int(_menu_boxes[0].size())) ) {
          getMenuboxElement(idmb, location + Vector2i(dx, dy*j));
        } else {
          getMenuboxElement(mb, location + Vector2i(dx, dy*j));
        }
      }
    }

    MenuBox* mbptr = mb.ptr;
    if( mbptr == _active_box ) {
      mbptr = nullptr;
      MenuBoxElement new_mbe;
      mb = new_mbe;
    }

    // Let's try the other direction
    if( !mbptr && dx > 0 && dy > 0 ) {
      if( dir == MenuDirections::left || dir == MenuDirections::right ) {
        dy = dy*(-1);
      } else if( dir == MenuDirections::up || dir == MenuDirections::down ) {
        dx = dx*(-1);
      }
      if( dir == MenuDirections::left || dir == MenuDirections::right ) {
        if( (int(_active_box_location.x() + dx*j) < 0)
             || (_active_box_location.x() + dx*j >= _menu_boxes[0].size()) ) {
          if( !idmb.ptr )
            getMenuboxElement(idmb, location + Vector2i(dx*j, dy*(-1)));
        } else {
          if( (int(_active_box_location.y() + dy*(-1)) < 0)
               || (_active_box_location.y() + dy*(-1) >= int(_menu_boxes.size())) ) {
            getMenuboxElement(idmb, location + Vector2i(dx*j, dy*(-1)));
          } else {
            getMenuboxElement(mb, location + Vector2i(dx*j, dy*(-1)));
          }
        }
      } else {
        if( (int(_active_box_location.y() + dy*j) < 0)
             || (_active_box_location.y() + dy*j >= _menu_boxes.size()) ) {
          if( !idmb.ptr )
            getMenuboxElement(idmb, location + Vector2i(dx, dy*j));
        } else {
          if( (int(_active_box_location.x() + dx) < 0)
               || (_active_box_location.x() + dx) >= int(_menu_boxes[0].size()) ) {
            getMenuboxElement(idmb, location + Vector2i(dx, dy*j));
          } else {
            getMenuboxElement(mb, location + Vector2i(dx, dy*j));
          }
        }
      }
    }

    if( mb.ptr && mb.ptr != _active_box )
      break;
  }
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
  _player->setRelativeBillboardRatio(relative_billboard_ratio);
}

void MusicMenu::draw() {
  _current_time = _current_time + _timeline.previousFrameDuration();
/*
  if( !_music->isPaused() ) {
    if( !_visualiser_loaded ) {
      if( !_visualiser_preparing )
        prepareVisualiser();
    } else {
      drawVisualiser();
    }
  }

  _songtitle->draw();
  _player->draw(_current_time);
*/
  for( auto i = _unsorted_menu_boxes.begin(); i < _unsorted_menu_boxes.end(); ++i) {
    (*i)->draw(_current_time);
  }

//  _music->draw();

  _timeline.nextFrame();
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
              OpenFF::MusicVisualiserChannel::channel_right,
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

  _magnitude_bin_matrix_left.reserve(_maximum_frame_count);
  _magnitude_bin_matrix_right.reserve(_maximum_frame_count);

  std::vector<std::vector<float>> temp_matrix;
  temp_matrix = _music->processAudioForMagnitudeVisualiser(
          OpenFF::MusicVisualiserChannel::channel_left,
          _frame_slice_left,
          _frame_slice_window);
  _magnitude_bin_matrix_left.insert(
          _magnitude_bin_matrix_left.begin(),
          temp_matrix.begin(),
          temp_matrix.end());
  temp_matrix = _music->processAudioForMagnitudeVisualiser(
          OpenFF::MusicVisualiserChannel::channel_right,
          _frame_slice_right,
          _frame_slice_window);
  _magnitude_bin_matrix_right.insert(
          _magnitude_bin_matrix_right.begin(),
          temp_matrix.begin(),
          temp_matrix.end());

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
          std::make_pair(&MusicMenu::pauseResume,InputEvents::music_pause),
          std::make_pair(&MusicMenu::menuUp,InputEvents::menu_up),
          std::make_pair(&MusicMenu::menuDown,InputEvents::menu_down),
          std::make_pair(&MusicMenu::menuLeft,InputEvents::menu_left),
          std::make_pair(&MusicMenu::menuRight,InputEvents::menu_right),
          std::make_pair(&MusicMenu::menuAcceptSelected,InputEvents::menu_accept),
          std::make_pair(&MusicMenu::menuExit,InputEvents::menu_exit)
      }
  );
}
