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

void Textbox::prepare() {
  prepareWindow();
  prepareText();
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
  if( _font != nullptr )
    _text = new Text(_font);
  else
    _text = new Text(_config->getFontLocation(), _config->getFontBaseSize());
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setViewportSize(_viewport_size);
  _text->setOffset(_offset);
}

void Textbox::draw() {
  _window->draw();
  if( _window->isFullyShown() ) _text->draw();
}
void Textbox::draw(std::string text) {
  _text->setText(text);
  draw();
}
void Textbox::write(std::string text) {
  _text->setText(text);
}

void Textbox::show() {
  _hidden = false;
  _window->show();
}
void Textbox::hide() {
  _hidden = true;
  _window->hide();
}

void Textbox::setFont(Font* font) {
  _font = font;
  delete(_text);
  prepareText();
}
void Textbox::setTextShadowType(OpenFF::ShadowTypes type) {
  _text->setShadowType(type);
}

void Textbox::setColor(Color3 color) {
  _window->setColor(color);
}
void Textbox::setColor(Color4 color) {
  _window->setColor(color);
}
// set color gradient, starting top-left going clockwise
void Textbox::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color) {
  _window->setColor(top_left_color, top_right_color, bottom_right_color, bottom_left_color);
}
void Textbox::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color,
        float alpha) {
  _window->setColor(top_left_color, top_right_color, bottom_right_color, bottom_left_color, alpha);
}
void Textbox::setBodyTransparency(float alpha) {
  _window->setBodyTransparency(alpha);
}
void Textbox::setBorderTransparency(float alpha) {
  _window->setBorderTransparency(alpha);
}

void Textbox::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _relative_billboard_ratio = relative_billboard_ratio;
  _window->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);
}
void Textbox::setViewportSize(Vector2i viewport_size) {
  _viewport_size = viewport_size;
  _window->setViewportSize(_viewport_size);
  _text->setViewportSize(_viewport_size);
}
void Textbox::setTextboxSize(Vector2i textbox_size) {
  _textbox_size = textbox_size;
  _window->setWindowSize(_textbox_size);
}
void Textbox::setOffset(Vector2i offset) {
  _offset = offset;
  _window->setOffset(_offset);
  _text->setOffset(_offset);
}

void Textbox::enableInstantRendering() {
  _window->enableInstantRendering();
}
void Textbox::disableInstantRendering() {
  _window->disableInstantRendering();
}
