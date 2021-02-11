#include "MenuBox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

MenuBox::MenuBox(
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Vector2i textbox_size,
        Vector2i offset) :
                _textbox_size(textbox_size),
                _offset(offset),
                _focus_normal(true),
                _selection_active(false),
                _selection_wobble_offset(0.5f),
                _selection_current_position_horizontally(0),
                _selection_current_position_vertically(0) {

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
  _focus->write()
         .setBorderImageLocation(config->getMenuFocusBorderLocation())
         .setBodyTransparency(0.0)
         .show();
}

MenuBox& MenuBox::enableSelection() {
  _selection_active = true;
  return *this;
}
MenuBox& MenuBox::disableSelection() {
  _selection_active = false;
  return *this;
}

MenuSelectionReturns MenuBox::selectionUp() {
  return MenuSelectionReturns::end_of_line;
}
MenuSelectionReturns MenuBox::selectionDown() {
  return MenuSelectionReturns::end_of_line;
}
MenuSelectionReturns MenuBox::selectionLeft() {
  if( _selection_current_position_horizontally > 0 ) {
    _focus->move(-1*Vector2i(_textbox->getMaximumCharacterWidth(),0));
    --_selection_current_position_horizontally;
    return MenuSelectionReturns::success;
  } else {
    return MenuSelectionReturns::end_of_line;
  }
  return MenuSelectionReturns::error;
}
MenuSelectionReturns MenuBox::selectionRight() {
  if( _selection_current_position_horizontally <
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
  return *this;
}
MenuBox& MenuBox::selectionCancel() {
  return *this;
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

MenuBox& MenuBox::write() {
  _textbox->write();
  return *this;
}
MenuBox& MenuBox::write(std::string text) {
  _textbox->write(text);
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

void MenuBox::draw() {
  _textbox->draw();
  if( _selection_active )
    _focus->draw();
}
void MenuBox::draw(float current_time) {
  this->updateSelectionWobble(current_time);
  this->draw();
}
