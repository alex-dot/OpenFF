#include "MenuBox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

MenuBox::MenuBox(
        MenuBoxType type,
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Vector2i textbox_size,
        Vector2i offset) :
                _type(type),
                _textbox_size(textbox_size),
                _offset(offset),
                _instances_in_menu_list(),
                _focus_normal(true),
                _selection_active(false),
                _selection_wobble_offset(0.5f),
                _selection_current_position_horizontally(0),
                _selection_current_position_vertically(0),
                _linked_boxes(),
                _menu_callbacks() {

  if( _type == MenuBoxType::freeform ) {
    _textbox = new OpenFF::FreeformTextbox(
            config,
            ressource_loader,
            relative_billboard_ratio,
            _textbox_size,
            _offset);
    _textbox->show();

    _focus = new OpenFF::Textbox(
            config,
            ressource_loader,
            relative_billboard_ratio,
            Vector2i(14,15),  // TODO calculate?
            offset+Vector2i(5,4));
  } else if( _type == MenuBoxType::regular ) {
    _textbox = new OpenFF::Textbox(
            config,
            ressource_loader,
            relative_billboard_ratio,
            _textbox_size,
            _offset);
    _textbox->show();

    _focus = new OpenFF::Textbox(
            config,
            ressource_loader,
            relative_billboard_ratio,
            Vector2i(textbox_size.x()-10,15),  // TODO calculate?
            offset+Vector2i(5,4));
  } else {
    FatlSrcLogicError("Tried to create menu box of unsupported type");
  }

  _focus->write()
         .setBorderImageLocation(config->getMenuFocusBorderLocation())
         .setBodyTransparency(0.0)
         .show();

  _linked_boxes.insert(std::pair<MenuDirections,OpenFF::MenuBox*>
          (MenuDirections::up,nullptr));
  _linked_boxes.insert(std::pair<MenuDirections,OpenFF::MenuBox*>
          (MenuDirections::down,nullptr));
  _linked_boxes.insert(std::pair<MenuDirections,OpenFF::MenuBox*>
          (MenuDirections::left,nullptr));
  _linked_boxes.insert(std::pair<MenuDirections,OpenFF::MenuBox*>
          (MenuDirections::right,nullptr));

  _instances_in_menu_list.push_back(Vector2i(1));
}

MenuBox::MenuBox(
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Vector2i textbox_size,
        Vector2i offset)
                : MenuBox::MenuBox(
                        MenuBoxType::regular,
                        config,
                        ressource_loader,
                        relative_billboard_ratio,
                        textbox_size,
                        offset) {}

MenuBox& MenuBox::enableSelection() {
  _focus->show();
  _selection_active = true;
  return *this;
}
MenuBox& MenuBox::disableSelection() {
  _focus->hide();
  _selection_active = false;
  return *this;
}

MenuSelectionReturns MenuBox::selectionUp() {
  unsigned int line_height = _textbox->getFontLineHeight();
  if( _type == MenuBoxType::regular )
    line_height += 1;

  if( _selection_current_position_vertically > 0 ) {
    --_selection_current_position_vertically;
    if( _selection_current_position_horizontally <= _textbox->getCharacterCountPerLine(_selection_current_position_vertically) ) {
      _focus->move(Vector2i(0,-1*line_height));
    } else {
      unsigned int horizontal_diff = -1 * (_selection_current_position_horizontally - _textbox->getCharacterCountPerLine(_selection_current_position_vertically));
      _focus->move(Vector2i(horizontal_diff*_textbox->getMaximumCharacterWidth(),-1*line_height));
      _selection_current_position_horizontally = _textbox->getCharacterCountPerLine(_selection_current_position_vertically);
    }
    return MenuSelectionReturns::success;
  } else {
    return MenuSelectionReturns::end_of_line;
  }
  return MenuSelectionReturns::error;
}
MenuSelectionReturns MenuBox::selectionDown() {
  unsigned int line_height = _textbox->getFontLineHeight();
  if( _type == MenuBoxType::regular )
    line_height += 1;

  if( _selection_current_position_vertically < _textbox->getLineCount() ) {
    ++_selection_current_position_vertically;
    if( _selection_current_position_horizontally <= _textbox->getCharacterCountPerLine(_selection_current_position_vertically) ) {
      _focus->move(Vector2i(0,line_height));
    } else {
      unsigned int horizontal_diff = -1 * (_selection_current_position_horizontally - _textbox->getCharacterCountPerLine(_selection_current_position_vertically));
      _focus->move(Vector2i(horizontal_diff*_textbox->getMaximumCharacterWidth(),line_height));
      _selection_current_position_horizontally = _textbox->getCharacterCountPerLine(_selection_current_position_vertically);
    }
    return MenuSelectionReturns::success;
  } else {
    return MenuSelectionReturns::end_of_line;
  }
  return MenuSelectionReturns::error;
}
MenuSelectionReturns MenuBox::selectionLeft() {
  if( _type == MenuBoxType::freeform && _selection_current_position_horizontally > 0 ) {
    _focus->move(-1*Vector2i(_textbox->getMaximumCharacterWidth(),0));
    --_selection_current_position_horizontally;
    return MenuSelectionReturns::success;
  } else {
    return MenuSelectionReturns::end_of_line;
  }
  return MenuSelectionReturns::error;
}
MenuSelectionReturns MenuBox::selectionRight() {
  if( _type == MenuBoxType::freeform && _selection_current_position_horizontally <
          _textbox->getCharacterCountPerLine(_selection_current_position_vertically) ) {
    _focus->move(Vector2i(_textbox->getMaximumCharacterWidth(),0));
    ++_selection_current_position_horizontally;
    return MenuSelectionReturns::success;
  } else {
    return MenuSelectionReturns::end_of_line;
  }
  return MenuSelectionReturns::error;
}
MenuBox& MenuBox::selectionAccept() {
  Vector2i pos = this->getCurrentPositionOfSelection();
  std::pair<OpenFF::MusicMenu*,std::function<void(OpenFF::MusicMenu&)>>
          callable = _menu_callbacks[pos.y()][pos.x()];
  if( callable.first != nullptr && callable.second != nullptr ) {
    (callable.second)(*(callable.first));
  } else {
    Err("Tried calling undefined menu callback: " + std::to_string(pos.y()) + std::to_string(pos.x()));
  }
  return *this;
}

MenuBox& MenuBox::setSelectionTop() {
  unsigned int vertical = 0;
  unsigned int horizontal = 0;
  if( _type == MenuBoxType::freeform ) {
    horizontal = _textbox->getMaximumCharacterWidth()*_selection_current_position_horizontally;
  }

  _focus->setOffset(_offset+Vector2i(5,4)+Vector2i(horizontal,vertical));
  _selection_current_position_vertically = 0;
  return *this;
}
MenuBox& MenuBox::setSelectionBottom() {
  unsigned int line_height = _textbox->getFontLineHeight();
  if( _type == MenuBoxType::regular )
    line_height += 1;
  unsigned int vertical = line_height*(_textbox->getLineCount()-_selection_current_position_vertically);

  unsigned int horizontal = 0;
  if( _type == MenuBoxType::freeform ) {
    horizontal = _textbox->getMaximumCharacterWidth()*_selection_current_position_horizontally;
  }

  _focus->setOffset(_offset+Vector2i(5,4)+Vector2i(horizontal,vertical));
  _selection_current_position_vertically = _textbox->getLineCount();
  return *this;
}
MenuBox& MenuBox::setSelectionLeftmost() {
  if( _type == MenuBoxType::freeform ) {
    unsigned int vertical = _textbox->getFontLineHeight()*_selection_current_position_vertically;
    unsigned int horizontal = 0;
    _focus->setOffset(_offset+Vector2i(5,4)+Vector2i(horizontal,vertical));
    _selection_current_position_horizontally = 0;
  }
  return *this;
}
MenuBox& MenuBox::setSelectionRightmost() {
  if( _type == MenuBoxType::freeform ) {
    _selection_current_position_horizontally =
          _textbox->getCharacterCountPerLine(_selection_current_position_vertically);
    unsigned int vertical =
          _textbox->getFontLineHeight()*_selection_current_position_vertically;
    unsigned int horizontal =
          _textbox->getMaximumCharacterWidth()*_selection_current_position_horizontally;
    _focus->setOffset(_offset+Vector2i(5,4)+Vector2i(horizontal,vertical));
  }
  return *this;
}
MenuBox& MenuBox::setSelectionAccept(
        Vector2i pos,
        OpenFF::MusicMenu* object,
        std::function<void(MusicMenu&)> functor) {
  if( pos.y() < int(_textbox->getLineCount()) || pos.y() == 0 ) {
    if( (  _type != MenuBoxType::freeform && pos.y() == 0 )
     || ( pos.x() <= int(_textbox->getCharacterCountPerLine(pos.y())) ) ) {
      _menu_callbacks[pos.y()][pos.x()] = std::make_pair<
            OpenFF::MusicMenu*,std::function<void(OpenFF::MusicMenu&)>>
            (std::move(object),std::move(functor));
    } else {
      DbgWarn{} << "Trying to set callback at undefined position:" << pos;
    }
  } else {
    DbgWarn{} << "Trying to set callback at undefined position:" << pos;
  }
  return *this;
}

MenuBox& MenuBox::setLinkedBox(MenuDirections dir, OpenFF::MenuBox* mb) {
  _linked_boxes[dir] = mb;
  return *this;
}
MenuBox& MenuBox::setLinkedBoxUp(OpenFF::MenuBox* mb) {
  return this->setLinkedBox(MenuDirections::up, mb);
}
MenuBox& MenuBox::setLinkedBoxDown(OpenFF::MenuBox* mb) {
  return this->setLinkedBox(MenuDirections::down, mb);
}
MenuBox& MenuBox::setLinkedBoxLeft(OpenFF::MenuBox* mb) {
  return this->setLinkedBox(MenuDirections::left, mb);
}
MenuBox& MenuBox::setLinkedBoxRight(OpenFF::MenuBox* mb) {
  return this->setLinkedBox(MenuDirections::right, mb);
}
MenuBox* MenuBox::getLinkedBox(MenuDirections dir) {
  return _linked_boxes[dir];
}
MenuBox* MenuBox::getLinkedBoxUp() {
  return this->getLinkedBox(MenuDirections::up);
}
MenuBox* MenuBox::getLinkedBoxDown() {
  return this->getLinkedBox(MenuDirections::down);
}
MenuBox* MenuBox::getLinkedBoxLeft() {
  return this->getLinkedBox(MenuDirections::left);
}
MenuBox* MenuBox::getLinkedBoxRight() {
  return this->getLinkedBox(MenuDirections::right);
}

MenuBox& MenuBox::updateSelectionWobble(float current_time) {
  if( _focus_normal && current_time-static_cast<int>(current_time) >= _selection_wobble_offset ) {
    _focus->shrink(2).move(Vector2i(1));
    _focus_normal = false;
  }
  if( !_focus_normal && current_time-static_cast<int>(current_time) < _selection_wobble_offset ) {
    _focus->expand(2).move(Vector2i(-1));
    _focus_normal = true;
  }

  return *this;
}
MenuBox& MenuBox::setSelectionWobbleOffset(float offset) {
  _selection_wobble_offset = offset;
  return *this;
}

MenuBox& MenuBox::setInstancesInMenuList(std::vector<Vector2i> instances) {
  _instances_in_menu_list = instances;
  return *this;
}
std::vector<Vector2i> MenuBox::getInstancesInMenuList() {
  return _instances_in_menu_list;
}
bool MenuBox::isSingleInstanceInMenuLust() {
  if( _instances_in_menu_list.size() > 1 )
    return false;
  return true;
}

MenuBox& MenuBox::write() {
  _textbox->write();
  return *this;
}
MenuBox& MenuBox::write(std::string text) {
  _textbox->write(text);

  _menu_callbacks.clear();
  Vector2i callable_matrix = this->getSelectableEntriesMatrix();
  std::pair<OpenFF::MusicMenu*,std::function<void(OpenFF::MusicMenu&)>>
          callable = std::make_pair(nullptr,nullptr);
  std::vector<std::pair<OpenFF::MusicMenu*,std::function<void(OpenFF::MusicMenu&)>>>
          cols(callable_matrix.x()+1,callable);
  std::vector<std::vector<std::pair<OpenFF::MusicMenu*,std::function<void(OpenFF::MusicMenu&)>>>>
          rows(callable_matrix.y()+1,cols);
  std::swap(rows,_menu_callbacks);

  return *this;
}

// Textbox functions
MenuBox& MenuBox::show() {
  _textbox->show();
  _focus->show();
  return *this;
}
MenuBox& MenuBox::hide() {
  _textbox->hide();
  _focus->hide();
  return *this;
}

// customize Textbox
// customize font and text
MenuBox& MenuBox::setFont(Font* font) {
  _textbox->setFont(font);
  return *this;
}
MenuBox& MenuBox::setFont(std::string location, int font_size) {
  _textbox->setFont(location, font_size);
  return *this;
}
MenuBox& MenuBox::setTextShadowType(OpenFF::ShadowTypes type) {
  _textbox->setTextShadowType(type);
  return *this;
}
// customize window
MenuBox& MenuBox::setTextboxBorderImageLocation(std::string location) {
  _textbox->setBorderImageLocation(location);
  return *this;
}
MenuBox& MenuBox::setFocusBorderImageLocation(std::string location) {
  _focus->setBorderImageLocation(location);
  return *this;
}
// set uniform color
MenuBox& MenuBox::setTextboxColor(Color3 color) {
  _textbox->setColor(color);
  return *this;
}
MenuBox& MenuBox::setTextboxColor(Color4 color) {
  _focus->setColor(color);
  return *this;
}
MenuBox& MenuBox::setFocusColor(Color3 color) {
  _textbox->setColor(color);
  return *this;
}
MenuBox& MenuBox::setFocusColor(Color4 color) {
  _focus->setColor(color);
  return *this;
}
// set color gradient, starting top-left going clockwise
MenuBox& MenuBox::setTextboxColor(Color3 tl, Color3 tr, Color3 br, Color3 bl) {
  _textbox->setColor(tl,tr,br,bl);
  return *this;
}
MenuBox& MenuBox::setTextboxColor(Color3 tl, Color3 tr, Color3 br, Color3 bl, float a) {
  _focus->setColor(tl,tr,br,bl,a);
  return *this;
}
MenuBox& MenuBox::setFocusColor(Color3 tl, Color3 tr, Color3 br, Color3 bl) {
  _textbox->setColor(tl,tr,br,bl);
  return *this;
}
MenuBox& MenuBox::setFocusColor(Color3 tl, Color3 tr, Color3 br, Color3 bl, float a) {
  _focus->setColor(tl,tr,br,bl,a);
  return *this;
}
// set transparency
MenuBox& MenuBox::setTextboxBodyTransparency(float a) {
  _textbox->setBodyTransparency(a);
  return *this;
}
MenuBox& MenuBox::setFocusBodyTransparency(float a) {
  _focus->setBodyTransparency(a);
  return *this;
}
MenuBox& MenuBox::setTextboxBorderTransparency(float a) {
  _textbox->setBorderTransparency(a);
  return *this;
}
MenuBox& MenuBox::setFocusBorderTransparency(float a) {
  _focus->setBorderTransparency(a);
  return *this;
}
// set generic textbox variables
MenuBox& MenuBox::enableInstantRendering() {
  _textbox->enableInstantRendering();
  _focus->enableInstantRendering();
  return *this;
}
MenuBox& MenuBox::disableInstantRendering() {
  _textbox->disableInstantRendering();
  _focus->disableInstantRendering();
  return *this;
}
MenuBox& MenuBox::setRelativeBillboardRatio(Vector2 ratio) {
  _textbox->setRelativeBillboardRatio(ratio);
  _focus->setRelativeBillboardRatio(ratio);
  return *this;
}
MenuBox& MenuBox::setViewportSize(Vector2i size) {
  _textbox->setViewportSize(size);
  _focus->setViewportSize(size);
  return *this;
}

MenuBox& MenuBox::setTextboxSize(Vector2i size) {
  _textbox->setTextboxSize(size);
  return *this;
}
MenuBox& MenuBox::setFocusSize(unsigned int size) {
  _focus->setTextboxSize(Vector2i(size));
  return *this;
}
MenuBox& MenuBox::expandTextbox(int value) {
  _textbox->expand(value);
  return *this;
}
MenuBox& MenuBox::expandFocus(int value) {
  _focus->expand(value);
  return *this;
}
MenuBox& MenuBox::shrinkTextbox(int value) {
  _textbox->shrink(value);
  return *this;
}
MenuBox& MenuBox::shrinkFocus(int value) {
  _focus->shrink(value);
  return *this;
}

Vector2i MenuBox::getOffset() {
  return _textbox->getOffset();
}
Vector2i MenuBox::getTextboxSize() {
  return _textbox->getTextboxSize();
}

MenuBox& MenuBox::rewriteCharacter(
        unsigned int line_index,
        unsigned int character_index,
        Vector2i offset_mod,
        std::string character) {
  _textbox->rewriteCharacter(line_index, character_index, offset_mod, character);
  return *this;
}
MenuBox& MenuBox::moveCharacter(
        unsigned int line_index,
        unsigned int character_index,
        Vector2i offset_mod) {
  _textbox->moveCharacter(line_index, character_index, offset_mod);
  return *this;
}
MenuBox& MenuBox::moveText(Vector2i offset) {
  _textbox->moveText(offset);
  return *this;
}

MenuBoxType MenuBox::getType() {
  return _type;
}

Vector2i MenuBox::getSelectableEntriesMatrix() {
  int lines = _textbox->getLineCount();
  unsigned int max_chars = 0;
  for( int i = 0; i <= lines; ++i ) {
    if( _textbox->getCharacterCountPerLine(i) > max_chars ) {
      max_chars = _textbox->getCharacterCountPerLine(i);
    }
  }
  return Vector2i(max_chars,lines);
}

unsigned int MenuBox::getCurrentHorizontalPositionOfSelection() {
  return _selection_current_position_horizontally;
}
unsigned int MenuBox::getCurrentVerticalPositionOfSelection() {
  return _selection_current_position_vertically;
}
Vector2i     MenuBox::getCurrentPositionOfSelection() {
  return Vector2i(this->getCurrentHorizontalPositionOfSelection(),
                  this->getCurrentVerticalPositionOfSelection());
}
Vector2i     MenuBox::getCurrentPixelPositionOfSelection() {
  unsigned int vertical =
    _textbox->getFontLineHeight()*_selection_current_position_vertically;
  unsigned int horizontal =
    _textbox->getMaximumCharacterWidth()*_selection_current_position_horizontally;
  return Vector2i(horizontal,vertical);
}
Vector2i     MenuBox::getRelativeSelectionOffset() {
  return _focus->getOffset()-_offset;
}
Vector2i     MenuBox::getSelectionOffset() {
  return _focus->getOffset();
}

void MenuBox::draw() {
  _textbox->draw();
  if( _selection_active )
    _focus->draw();
}
void MenuBox::draw(float current_time) {
  this->updateSelectionWobble(current_time);
  this->draw();
}
