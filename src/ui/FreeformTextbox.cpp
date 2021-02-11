#include "FreeformTextbox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

FreeformTextbox::FreeformTextbox(
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Vector2i textbox_size,
        Vector2i offset)
        : AbstractTextbox::AbstractTextbox(
                config,
                ressource_loader,
                relative_billboard_ratio,
                textbox_size,
                offset) {
  prepare();
}

void FreeformTextbox::prepareText() {
  setBorderOffset(Vector2i(7));
  _line_height = _font->getFontLineHeight();
  // always divided by 2 because the viewport goes from (-1,1)
  _max_char_width = _font->getMaximumGlyphWidth()/2;
}

FreeformTextbox& FreeformTextbox::write(std::string text) {
  if( text == "" )
    this->write(" ");

  std::string linebreak = "\n";
  // hacky way to make the loop work ('cause the last line is always ignored)
  text = text + linebreak;
  std::string line;
  int start = 0;
  auto end = text.find(linebreak);

  // loop over lines
  while( end != std::string::npos ) {
    unsigned current_line_index = _textmap.size();

    line = text.substr(start, end - start);
    std::vector<FreeformTextboxCharacter> newline;

    // loop over characters
    for( unsigned i = 0; i < line.length(); ++i ) {
      unsigned current_char_index = newline.size();
      unsigned char_length = getCharLength(line.substr(i,1));

      FreeformTextboxCharacter character;
      character.text_obj = new Text(_font);
      character.offset = this->calculateCharacterOffset(
              character.text_obj,
              line.substr(i,char_length),
              current_line_index,
              current_char_index);
      character.offset_mod = Vector2i(0,0);

      character.text_obj->setRelativeBillboardRatio(_relative_billboard_ratio);
      character.text_obj->setViewportSize(_viewport_size);
      character.text_obj->setOffset(_offset);
      character.text_obj->setBorderOffset(character.offset+character.offset_mod);
      character.text_obj->setText(line.substr(i,char_length));

      newline.push_back(character);

      i = i + char_length - 1;
    }

    start = end + linebreak.length();
    end = text.find(linebreak, start);
    _textmap.push_back(newline);
  }

  return *this;
}
FreeformTextbox& FreeformTextbox::write() {
  this->write(" ");
  return *this;
}

FreeformTextbox& FreeformTextbox::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _relative_billboard_ratio = relative_billboard_ratio;
  _window->setRelativeBillboardRatio(_relative_billboard_ratio);
  if( _window->isFullyShown() ) {
    for( auto i = _textmap.begin(); i < _textmap.end(); ++i ) {
      for( auto j = i->begin(); j < i->end(); ++j ) {
        j->text_obj->setRelativeBillboardRatio(_relative_billboard_ratio);
      }
    }
  }

  return *this;
}

FreeformTextbox& FreeformTextbox::rewriteCharacter(
        unsigned int line_index,
        unsigned int character_index,
        Vector2i offset_mod,
        std::string character) {
  if( line_index < _textmap.size() && character_index < _textmap[line_index].size() ) {
    _textmap[line_index][character_index].offset = this->calculateCharacterOffset(
            _textmap[line_index][character_index].text_obj,
            character,
            line_index,
            character_index);
    _textmap[line_index][character_index].offset_mod = offset_mod;
    _textmap[line_index][character_index].text_obj->setBorderOffset(
            _textmap[line_index][character_index].offset+_textmap[line_index][character_index].offset_mod);
    _textmap[line_index][character_index].text_obj->setText(character);
  } else {
    Err<std::string>("Trying to write non-existing character.");
  }

  return *this;
}

FreeformTextbox& FreeformTextbox::moveCharacter(
        unsigned int line_index,
        unsigned int character_index,
        Vector2i offset_mod) {
  if( line_index < _textmap.size() && character_index < _textmap[line_index].size() ) {
    std::string character = _textmap[line_index][character_index].text_obj->getText();
    this->rewriteCharacter(
            line_index,
            character_index,
            offset_mod,
            character);
  } else {
    Err<std::string>("Trying to move non-existing character.");
  }
  return *this;
}

FreeformTextbox& FreeformTextbox::moveText(Vector2i offset) {
  _border_offset = _border_offset + offset;
  return *this;
}

unsigned int FreeformTextbox::getLineCount() {
  return _textmap.size()-1;
}
unsigned int FreeformTextbox::getCharacterCountPerLine(unsigned int line) {
  return _textmap[line].size()-1;
}
unsigned int FreeformTextbox::getMaximumCharacterWidth() {
  return _max_char_width;
}

FreeformTextbox& FreeformTextbox::draw() {
  _window->draw();
  if( _window->isFullyShown() ) {
    for( auto i = _textmap.begin(); i < _textmap.end(); ++i ) {
      for( auto j = i->begin(); j < i->end(); ++j ) {
        j->text_obj->draw();
      }
    }
  }

  return *this;
}

unsigned int FreeformTextbox::calculateCharacterWidth(
        Text* text_obj,
        std::string character) {
  unsigned char_width = text_obj->getFontGlyphWidth(character);
  return (_max_char_width - char_width)/2;
}

Vector2i FreeformTextbox::calculateCharacterOffset(
        unsigned int line_index,
        unsigned int character_index,
        unsigned int width) {
  unsigned offset_x = _border_offset.x() + _max_char_width * character_index + width;
  unsigned offset_y = _border_offset.y() + _line_height * line_index;
  return Vector2i(offset_x, offset_y);
}

Vector2i FreeformTextbox::calculateCharacterOffset(
        Text* text_obj,
        std::string character,
        unsigned int line_index,
        unsigned int character_index) {
  unsigned int width = this->calculateCharacterWidth(text_obj, character);
  Vector2i offset = this->calculateCharacterOffset(line_index, character_index, width);
  return offset;
}
