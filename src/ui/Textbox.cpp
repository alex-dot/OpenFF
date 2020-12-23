#include "Textbox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

Textbox::Textbox(
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

void Textbox::prepareText() {
  _text = new Text(_font);
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setViewportSize(_viewport_size);
  _text->setOffset(_offset);
}

Textbox& Textbox::write(std::string text) {
  if( text != "" )
    _text->setText(text);
  else
    _text->setText(" ");

  return *this;
}
Textbox& Textbox::write() {
  this->write(" ");
  return *this;
}
