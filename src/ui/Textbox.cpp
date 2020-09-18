#include "Textbox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

void Textbox::prepareText() {
  _text = new Text(_font);
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setViewportSize(_viewport_size);
  _text->setOffset(_offset);
}

Textbox& Textbox::write(std::string text) {
  _text->setText(text);

  return *this;
}
