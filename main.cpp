#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "skia/include/gpu/ganesh/GrBackendSurface.h"
#include "skia/include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "skia/include/gpu/ganesh/gl/GrGLBackendSurface.h"

#include "skia/include/core/SkPaint.h"
#include "skia/include/core/SkCanvas.h"
#include "skia/include/core/SkColorSpace.h"

#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"

#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"

void draw_line(SkCanvas *canvas)
{
    SkPaint p;

    p.setColor(SK_ColorRED);
    p.setAntiAlias(true);
    p.setStyle(SkPaint::kStroke_Style);
    p.setStrokeWidth(10);

    canvas->drawLine(20, 20, 100, 100, p);
}

void draw_square(SkCanvas *canvas)
{
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    canvas->drawRect({0, 0, 250, 250}, paint);

    auto [width, height] = canvas->imageInfo().dimensions();
    canvas->drawRect({width - 250.0f, height - 250.0f, (float)width, (float)height}, paint);
}

#define WIDTH 1280
#define HEIGHT 720

static auto inline CreateSurface(auto &context, auto width, auto height)
{
    auto target = GrBackendRenderTargets::MakeGL(
        width, height,
        0, // sample count
        0, // stencil bits
        (GrGLFramebufferInfo){
            .fFBOID = 0,
            .fFormat = GL_RGBA8,
        });
    return SkSurfaces::WrapBackendRenderTarget(
        context.get(),
        target,
        GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin,
        SkColorType::kRGBA_8888_SkColorType,
        nullptr,
        nullptr);
}

int main(int, char **)
{
    auto window = sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "CMake SFML Project");
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(true);
    (void)window.setActive(true);

    sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
    sk_sp<GrDirectContext> context = GrDirectContexts::MakeGL(interface);
    sk_sp<SkSurface> surface = CreateSurface(context, WIDTH, HEIGHT);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *resized = event->getIf<sf::Event::Resized>())
            {
                std::printf("RESIZE\n");
                glViewport(0, 0, resized->size.x, resized->size.y);
                surface = CreateSurface(context, resized->size.x, resized->size.y);
            }
        }

        surface->getCanvas()->clear(SK_ColorDKGRAY);

        draw_square(surface->getCanvas());
        draw_line(surface->getCanvas());

        context->flush();

        window.display();
    }
}