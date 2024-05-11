/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace yup
{

//==============================================================================
namespace {

rive::StrokeJoin toStrokeJoin (StrokeJoin join) noexcept
{
    return static_cast<rive::StrokeJoin> (join);
}

rive::StrokeCap toStrokeCap (StrokeCap cap) noexcept
{
    return static_cast<rive::StrokeCap> (cap);
}

rive::RawPath toRawPath (Path path)
{
    rive::RawPath rawPath;

    for (const auto& segment : path)
    {
        if (segment.type == Path::SegmentType::MoveTo)
            rawPath.moveTo (segment.x, segment.y);

        else if (segment.type == Path::SegmentType::LineTo)
            rawPath.lineTo (segment.x, segment.y);

        else if (segment.type == Path::SegmentType::QuadTo)
            rawPath.quadTo (segment.x, segment.y,
                            segment.x1, segment.y1);

        else if (segment.type == Path::SegmentType::CubicTo)
            rawPath.cubicTo (segment.x, segment.y,
                             segment.x1, segment.y1,
                             segment.x2, segment.y2);
    }

    return rawPath;
}

rive::RawPath toRawPath (Path path, float translateX, float translateY)
{
    rive::RawPath rawPath;

    for (const auto& segment : path)
    {
        if (segment.type == Path::SegmentType::MoveTo)
            rawPath.moveTo (segment.x + translateX, segment.y + translateY);

        else if (segment.type == Path::SegmentType::LineTo)
            rawPath.lineTo (segment.x + translateX, segment.y + translateY);

        else if (segment.type == Path::SegmentType::QuadTo)
            rawPath.quadTo (segment.x + translateX, segment.y + translateY,
                            segment.x1 + translateX, segment.y1 + translateY);

        else if (segment.type == Path::SegmentType::CubicTo)
            rawPath.cubicTo (segment.x + translateX, segment.y + translateY,
                             segment.x1 + translateX, segment.y1 + translateY,
                             segment.x2 + translateX, segment.y2 + translateY);
    }

    return rawPath;
}

rive::rcp<rive::RenderShader> toColorGradient (rive::Factory& factory, const ColorGradient& gradient)
{
    const uint32 colors[] = { gradient.getStartColor(), gradient.getFinishColor() };
    const float stops[] = { gradient.getStartDelta(), gradient.getFinishDelta() };

    if (gradient.getType() == ColorGradient::Linear)
    {
        return factory.makeLinearGradient (gradient.getStartX(),
                                           gradient.getStartY(),
                                           gradient.getFinishX(),
                                           gradient.getFinishY(),
                                           colors,
                                           stops,
                                           2);
    }
    else
    {
        return factory.makeRadialGradient (gradient.getStartX(),
                                           gradient.getStartY(),
                                           gradient.getRadius(),
                                           colors,
                                           stops,
                                           2);
    }
}

} // namespace

//==============================================================================
Graphics::SavedState::SavedState (Graphics& g)
    : g (std::addressof (g))
{
}

Graphics::SavedState::SavedState (SavedState&& other)
    : g (std::exchange (other.g, nullptr))
{
}

Graphics::SavedState& Graphics::SavedState::operator= (SavedState&& other)
{
    g = std::exchange (other.g, nullptr);
    return *this;
}

Graphics::SavedState::~SavedState()
{
    if (g != nullptr)
        g->restoreState();
}

//==============================================================================
Graphics::Graphics (GraphicsContext& context, rive::Renderer& renderer) noexcept
    : context (context)
    , factory (*context.factory())
    , renderer (renderer)
{
    renderOptions.emplace_back();
}

//==============================================================================
rive::Factory* Graphics::getFactory()
{
    return std::addressof (factory);
}

rive::Renderer* Graphics::getRenderer()
{
    return std::addressof (renderer);
}

//==============================================================================
Graphics::RenderOptions& Graphics::currentRenderOptions()
{
    jassert (! renderOptions.empty());

    return renderOptions.back();
}

const Graphics::RenderOptions& Graphics::currentRenderOptions() const
{
    jassert (! renderOptions.empty());

    return renderOptions.back();
}

//==============================================================================
Graphics::SavedState Graphics::saveState()
{
    jassert (! renderOptions.empty());

    renderOptions.emplace_back (renderOptions.back());

    renderer.save();

    return { *this };
}

void Graphics::restoreState()
{
    renderer.restore();

    renderOptions.pop_back();
}

//==============================================================================
void Graphics::setColor (Color color)
{
    currentRenderOptions().color = color;
    currentRenderOptions().isCurrentBrushColor = true;
}

Color Graphics::getColor() const
{
    return currentRenderOptions().color;
}

void Graphics::setColorGradient (ColorGradient gradient)
{
    currentRenderOptions().gradient = std::move (gradient);
    currentRenderOptions().isCurrentBrushColor = false;
}

ColorGradient Graphics::getColorGradient() const
{
    return currentRenderOptions().gradient;
}

void Graphics::setOpacity (uint8 opacity)
{
    currentRenderOptions().color.setAlpha (opacity);
    currentRenderOptions().gradient.setAlpha (opacity);
}

uint8 Graphics::getOpacity() const
{
    return currentRenderOptions().color.getAlpha();
}

void Graphics::setStrokeJoin (StrokeJoin join)
{
    currentRenderOptions().join = join;
}

StrokeJoin Graphics::getStrokeJoin() const
{
    return currentRenderOptions().join;
}

void Graphics::setStrokeCap (StrokeCap cap)
{
    currentRenderOptions().cap = cap;
}

StrokeCap Graphics::getStrokeCap() const
{
    return currentRenderOptions().cap;
}

void Graphics::setDrawingArea (const Rectangle<float>& drawingArea)
{
    currentRenderOptions().drawingArea = drawingArea;
}

Rectangle<float> Graphics::getDrawingArea() const
{
    return currentRenderOptions().drawingArea;
}

//==============================================================================
void Graphics::drawLine (float x1, float y1, float x2, float y2, float thickness)
{
    const auto& options = currentRenderOptions();

    rive::RawPath rawPath;
    rawPath.moveTo (options.translateX (x1), options.translateY (y1));
    rawPath.lineTo (options.translateX (x2), options.translateY (y2));

    renderDrawPath (rawPath, options, thickness);
}

void Graphics::drawLine (const Point<float>& p1, const Point<float>& p2, float thickness)
{
    drawLine (p1.getX(), p1.getY(), p2.getX(), p2.getY(), thickness);
}

//==============================================================================
void Graphics::fillRect (float x, float y, float width, float height)
{
    const auto& options = currentRenderOptions();

    rive::RawPath rawPath;
    rawPath.addRect (rive::AABB (
        options.translateX (x),
        options.translateY (y),
        options.translateX (x) + width,
        options.translateY (y) + height));

    renderFillPath (rawPath, options);
}

void Graphics::fillRect (const Rectangle<float>& r)
{
    fillRect (r.getX(), r.getY(), r.getWidth(), r.getHeight());
}

//==============================================================================
void Graphics::drawRect (float x, float y, float width, float height, float thickness)
{
    const auto& options = currentRenderOptions();

    rive::RawPath rawPath;
    rawPath.addRect (rive::AABB (
        options.translateX (x),
        options.translateY (y),
        options.translateX (x) + width,
        options.translateY (y) + height));

    renderDrawPath (rawPath, options, thickness);
}

void Graphics::drawRect (const Rectangle<float>& r, float thickness)
{
    drawRect (r.getX(), r.getY(), r.getWidth(), r.getHeight(), thickness);
}

//==============================================================================
void Graphics::fillRoundedRect (float x, float y, float width, float height, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight)
{
    const auto& options = currentRenderOptions();

    Path path;
    path.addRoundedRectangle (
        options.translateX (x),
        options.translateY (y),
        width, height,
        radiusTopLeft, radiusTopRight, radiusBottomLeft, radiusBottomRight);

    auto rawPath = toRawPath (path);
    renderFillPath (rawPath, options);
}

void Graphics::fillRoundedRect (float x, float y, float width, float height, float radius)
{
    fillRoundedRect (x, y, width, height, radius, radius, radius, radius);
}

void Graphics::fillRoundedRect (const Rectangle<float>& r, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight)
{
    fillRoundedRect (r.getX(), r.getY(), r.getWidth(), r.getHeight(), radiusTopLeft, radiusTopRight, radiusBottomLeft, radiusBottomRight);
}

void Graphics::fillRoundedRect (const Rectangle<float>& r, float radius)
{
    fillRoundedRect (r.getX(), r.getY(), r.getWidth(), r.getHeight(), radius, radius, radius, radius);
}

//==============================================================================
void Graphics::drawRoundedRect (float x, float y, float width, float height, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight, float thickness)
{
    const auto& options = currentRenderOptions();

    Path path;
    path.addRoundedRectangle (
        options.translateX (x),
        options.translateY (y),
        width, height,
        radiusTopLeft, radiusTopRight, radiusBottomLeft, radiusBottomRight);

    auto rawPath = toRawPath (path);
    renderDrawPath (rawPath, options, thickness);
}

void Graphics::drawRoundedRect (float x, float y, float width, float height, float radius, float thickness)
{
    drawRoundedRect (x, y, width, height, radius, radius, radius, radius, thickness);
}

void Graphics::drawRoundedRect (const Rectangle<float>& r, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight, float thickness)
{
    drawRoundedRect (r.getX(), r.getY(), r.getWidth(), r.getHeight(), radiusTopLeft, radiusTopRight, radiusBottomLeft, radiusBottomRight, thickness);
}

void Graphics::drawRoundedRect (const Rectangle<float>& r, float radius, float thickness)
{
    drawRoundedRect (r.getX(), r.getY(), r.getWidth(), r.getHeight(), radius, radius, radius, radius, thickness);
}

//==============================================================================
void Graphics::drawPath (const Path& path, float thickness)
{
    const auto& options = currentRenderOptions();

    auto rawPath = toRawPath (path, options.translateX (0.0f), options.translateY (0.0f));
    renderDrawPath (rawPath, options, thickness);
}

//==============================================================================
void Graphics::fillPath (const Path& path)
{
    const auto& options = currentRenderOptions();

    auto rawPath = toRawPath (path, options.translateX (0.0f), options.translateY (0.0f));
    renderFillPath (rawPath, options);
}

//==============================================================================
void Graphics::renderDrawPath (rive::RawPath& rawPath, const RenderOptions& options, float thickness)
{
    auto paint = factory.makeRenderPaint();
    paint->style (rive::RenderPaintStyle::stroke);
    paint->thickness (thickness);
    paint->join (toStrokeJoin (options.join));
    paint->cap (toStrokeCap (options.cap));

    if (options.isColor())
        paint->color (options.getColor());
    else
        paint->shader (toColorGradient (factory, options.getColorGradient()));

    auto renderPath = factory.makeRenderPath (rawPath, rive::FillRule::nonZero);
    renderer.drawPath (renderPath.get(), paint.get());
}

void Graphics::renderFillPath (rive::RawPath& rawPath, const RenderOptions& options)
{
    auto paint = factory.makeRenderPaint();
    paint->style (rive::RenderPaintStyle::fill);

    if (options.isColor())
        paint->color (options.getColor());
    else
        paint->shader (toColorGradient (factory, options.getColorGradient()));

    auto renderPath = factory.makeRenderPath (rawPath, rive::FillRule::nonZero);
    renderer.drawPath (renderPath.get(), paint.get());
}
} // namespace yup
