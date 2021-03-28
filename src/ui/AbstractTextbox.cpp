#include "AbstractTextbox.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

AbstractTextbox::AbstractTextbox() :
        _text(nullptr),
        _window(nullptr),
        _font(nullptr),
        _border_image_location(""),
        _relative_billboard_ratio(Vector2(1)),
        _viewport_size(default_viewport),
        _textbox_size(Vector2i(1)),
        _offset(Vector2i(1)),
        _hidden(true) {}
AbstractTextbox::AbstractTextbox(Configuration* config, RessourceLoader* ressource_loader) :
        AbstractTextbox::AbstractTextbox() {
  _config = config;
  _ressource_loader = ressource_loader;

  if( _config->getFontCacheGlyphTexture() ) {
    _font = _ressource_loader->getFont(
              _config->getFontLocation(),
              _config->getFontBaseSize(),
              _config->getFontGlyphTextureLocation());
  } else {
    _font = _ressource_loader->getFont(
              _config->getFontLocation(),
              _config->getFontBaseSize());
  }
}
AbstractTextbox::AbstractTextbox(
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Vector2i textbox_size,
        Vector2i offset) :
                AbstractTextbox::AbstractTextbox(config, ressource_loader) {
  _relative_billboard_ratio = relative_billboard_ratio;
  _textbox_size = textbox_size;
  _offset = offset;
}

AbstractTextbox& AbstractTextbox::prepare() {
  prepareWindow();
  prepareText();

  return *this;
}
void AbstractTextbox::prepareWindow() {
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

AbstractTextbox& AbstractTextbox::draw() {
  _window->draw();
  if( _window->isFullyShown() ) _text->draw();

  return *this;
}
AbstractTextbox& AbstractTextbox::draw(std::string text) {
  write(text);
  draw();

  return *this;
}

AbstractTextbox& AbstractTextbox::show() {
  _hidden = false;
  _window->show();

  return *this;
}
AbstractTextbox& AbstractTextbox::hide() {
  _hidden = true;
  _window->hide();
  _text->hide();

  return *this;
}

AbstractTextbox& AbstractTextbox::setFont(Font* font) {
  _font = font;
  delete(_text);
  prepareText();

  return *this;
}
AbstractTextbox& AbstractTextbox::setFont(std::string location, int base_size) {
  _font = _ressource_loader->getFont(location, base_size);

  return *this;
}

AbstractTextbox& AbstractTextbox::setBorderImageLocation(std::string location) {
  Containers::Optional<Trade::ImageData2D> image;
  _ressource_loader->getImage(location, image);
  _window->setBorder(image);

  return *this;
}

AbstractTextbox& AbstractTextbox::setTextShadowType(OpenFF::ShadowTypes type) {
  _text->setShadowType(type);

  return *this;
}

AbstractTextbox& AbstractTextbox::setColor(Color3 color) {
  _window->setColor(color);

  return *this;
}
AbstractTextbox& AbstractTextbox::setColor(Color4 color) {
  _window->setColor(color);

  return *this;
}
// set color gradient, starting top-left going clockwise
AbstractTextbox& AbstractTextbox::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color) {
  _window->setColor(top_left_color, top_right_color, bottom_right_color, bottom_left_color);

  return *this;
}
AbstractTextbox& AbstractTextbox::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color,
        float alpha) {
  _window->setColor(top_left_color, top_right_color, bottom_right_color, bottom_left_color, alpha);

  return *this;
}
AbstractTextbox& AbstractTextbox::setBodyTransparency(float alpha) {
  _window->setBodyTransparency(alpha);

  return *this;
}
AbstractTextbox& AbstractTextbox::setBorderTransparency(float alpha) {
  _window->setBorderTransparency(alpha);

  return *this;
}

AbstractTextbox& AbstractTextbox::enableInstantRendering() {
  _window->enableInstantRendering();
  _text->enableInstantRendering();

  return *this;
}
AbstractTextbox& AbstractTextbox::disableInstantRendering() {
  _window->disableInstantRendering();
  _text->disableInstantRendering();

  return *this;
}
AbstractTextbox& AbstractTextbox::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _relative_billboard_ratio = relative_billboard_ratio;
  _window->setRelativeBillboardRatio(_relative_billboard_ratio);
  _text->setRelativeBillboardRatio(_relative_billboard_ratio);

  return *this;
}
AbstractTextbox& AbstractTextbox::setViewportSize(Vector2i viewport_size) {
  _viewport_size = viewport_size;
  _window->setViewportSize(_viewport_size);
  _text->setViewportSize(_viewport_size);

  return *this;
}
AbstractTextbox& AbstractTextbox::setResizeFactor(float factor) {
  _window->setResizeFactor(factor);
  _text->setResizeFactor(factor);
  this->disableRelativeBillboardRendering();

  return *this;
}
AbstractTextbox& AbstractTextbox::enableRelativeBillboardRendering() {
  _window->enableRelativeBillboardRendering();
  _text->enableRelativeBillboardRendering();

  return *this;
}
AbstractTextbox& AbstractTextbox::disableRelativeBillboardRendering() {
  _window->disableRelativeBillboardRendering();
  _text->disableRelativeBillboardRendering();

  return *this;
}

AbstractTextbox& AbstractTextbox::setTextboxSize(Vector2i textbox_size) {
  _textbox_size = textbox_size;
  _window->setWindowSize(_textbox_size);

  return *this;
}
AbstractTextbox& AbstractTextbox::expand(int pixel_factor) {
  this->setTextboxSize(_textbox_size + Vector2i(pixel_factor));

  return *this;
}
AbstractTextbox& AbstractTextbox::shrink(int pixel_factor) {
  this->expand(-1*pixel_factor);

  return *this;
}

AbstractTextbox& AbstractTextbox::setOffset(Vector2i offset) {
  _offset = offset;
  _window->setOffset(_offset);
  _text->setOffset(_offset);

  return *this;
}
Vector2i AbstractTextbox::getOffset() {
  return _offset;
}
Vector2i AbstractTextbox::getTextboxSize() {
  return _textbox_size;
}
AbstractTextbox& AbstractTextbox::move(Vector2i pixels) {
  this->setOffset(_offset + pixels);

  return *this;
}
AbstractTextbox& AbstractTextbox::setBorderOffset(Vector2i offset) {
  _border_offset = offset;

  return *this;
}

unsigned int AbstractTextbox::getLineCount() {
  std::string text = _text->getText();
  return std::count(text.begin(), text.end(), '\n');
}
unsigned int AbstractTextbox::getCharacterCountPerLine(unsigned int) {
  // by default, this returns 0, i.e. N/A or "use windowSize instead"
  return 0;
}
unsigned int AbstractTextbox::getMaximumCharacterWidth() {
  // by default, this returns 0, i.e. N/A or "use windowSize instead"
  return 0;
}
unsigned int AbstractTextbox::getFontLineHeight() {
  return _font->getFontLineHeight();
}

AbstractTextbox& AbstractTextbox::rewriteCharacter(unsigned int, unsigned int, Vector2i, std::string) {
  DbgWarn{} << "Rewriting character not supported by Textbox, use FreeformTextbox instead";
  return *this;
}
AbstractTextbox& AbstractTextbox::moveCharacter(unsigned int, unsigned int, Vector2i) {
  DbgWarn{} << "Moving character not supported by Textbox, use FreeformTextbox instead";
  return *this;
}
AbstractTextbox& AbstractTextbox::moveText(Vector2i) {
  DbgWarn{} << "Moving text not supported by Textbox, use FreeformTextbox instead";
  return *this;
}
