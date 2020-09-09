#include "Textbox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

Textbox::Textbox() :
        _text(nullptr),
        _window(nullptr),
        _font(nullptr),
        _relative_billboard_ratio(Vector2(1)),
        _viewport_size(default_viewport),
        _textbox_size(Vector2i(1)),
        _offset(Vector2i(1)),
        _hidden(true) {}
Textbox::Textbox(Configuration* config, RessourceLoader* ressource_loader) :
        Textbox::Textbox() {
  _config = config;
  _ressource_loader = ressource_loader;

  _font = _ressource_loader->getFont(
            _config->getFontLocation(),
            _config->getFontBaseSize());
}
Textbox::Textbox(
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Vector2i textbox_size,
        Vector2i offset) :
                Textbox::Textbox(config, ressource_loader) {
  _relative_billboard_ratio = relative_billboard_ratio;
  _textbox_size = textbox_size;
  _offset = offset;

  // since everything necessary is initialised, call prepare() directly
  prepare();
}

Textbox& Textbox::prepare() {
  prepareWindow();
  prepareText();

  return *this;
}
void Textbox::prepareWindow() {
  _window = new OpenFF::Window(_viewport_size);
  _window->setRelativeBillboardRatio(_relative_billboard_ratio);
  _window->setWindowSize(_textbox_size);
  _window->setOffset(_offset);

  using namespace Magnum::Math::Literals;
  //_window->setColor(0x000080ff_rgbaf);
  _window->setColor(0x0000b0_rgbf,0x000080_rgbf,0x000020_rgbf,0x000050_rgbf);
  //_window->setBodyTransparency(0.5);
  //_window->setBorderTransparency(0.5);

  Containers::Optional<Trade::ImageData2D> image;
  _ressource_loader->getImage(_config->getBorderLocation(), image);
  _window->setBorder(image);
}
void Textbox::prepareText() {
  _text = new Text(_font);
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setViewportSize(_viewport_size);
  _text->setOffset(_offset);
}

Textbox& Textbox::draw() {
  _window->draw();
  if( _window->isFullyShown() ) _text->draw();

  return *this;
}
Textbox& Textbox::draw(std::string text) {
  _text->setText(text);
  draw();

  return *this;
}
Textbox& Textbox::write(std::string text) {
  _text->setText(text);

  return *this;
}

Textbox& Textbox::show() {
  _hidden = false;
  _window->show();

  return *this;
}
Textbox& Textbox::hide() {
  _hidden = true;
  _window->hide();
  _text->hide();

  return *this;
}

Textbox& Textbox::setFont(Font* font) {
  _font = font;
  delete(_text);
  prepareText();

  return *this;
}

Textbox& Textbox::setTextShadowType(OpenFF::ShadowTypes type) {
  _text->setShadowType(type);

  return *this;
}

Textbox& Textbox::setColor(Color3 color) {
  _window->setColor(color);

  return *this;
}
Textbox& Textbox::setColor(Color4 color) {
  _window->setColor(color);

  return *this;
}
// set color gradient, starting top-left going clockwise
Textbox& Textbox::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color) {
  _window->setColor(top_left_color, top_right_color, bottom_right_color, bottom_left_color);

  return *this;
}
Textbox& Textbox::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color,
        float alpha) {
  _window->setColor(top_left_color, top_right_color, bottom_right_color, bottom_left_color, alpha);

  return *this;
}
Textbox& Textbox::setBodyTransparency(float alpha) {
  _window->setBodyTransparency(alpha);

  return *this;
}
Textbox& Textbox::setBorderTransparency(float alpha) {
  _window->setBorderTransparency(alpha);

  return *this;
}

Textbox& Textbox::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _relative_billboard_ratio = relative_billboard_ratio;
  _window->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);

  return *this;
}
Textbox& Textbox::setViewportSize(Vector2i viewport_size) {
  _viewport_size = viewport_size;
  _window->setViewportSize(_viewport_size);
  _text->setViewportSize(_viewport_size);

  return *this;
}
Textbox& Textbox::setTextboxSize(Vector2i textbox_size) {
  _textbox_size = textbox_size;
  _window->setWindowSize(_textbox_size);

  return *this;
}
Textbox& Textbox::setOffset(Vector2i offset) {
  _offset = offset;
  _window->setOffset(_offset);
  _text->setOffset(_offset);

  return *this;
}

Textbox& Textbox::enableInstantRendering() {
  _window->enableInstantRendering();
  _text->enableInstantRendering();

  return *this;
}
Textbox& Textbox::disableInstantRendering() {
  _window->disableInstantRendering();
  _text->disableInstantRendering();

  return *this;
}
