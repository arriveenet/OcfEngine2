#include "Texture.h"
#include <stdlib.h>
#include <math.h>

using namespace ocf;

#define DOT(a, b) (a[0] * b[0] + a[1] * b[1] + a[2] * b[2])

#define B 256

static int p[B + B + 2];
static float g[B + B + 2][3];
static int start = 1;

#define setup(i, b0, b1, r0, r1)                                                                   \
    t = vec[i] + 10000.;                                                                           \
    b0 = ((int)t) & (B - 1);                                                                       \
    b1 = (b0 + 1) & (B - 1);                                                                       \
    r0 = t - (int)t;                                                                               \
    r1 = r0 - 1.;

void init();

static float noise3(float vec[3])
{
    int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
    float rx0, rx1, ry0, ry1, rz0, rz1, *q, sx, sy, sz, a, b, c, d, t, u, v;
    register int i, j;

    if (start) {
        start = 0;
        init();
    }

    setup(0, bx0, bx1, rx0, rx1);
    setup(1, by0, by1, ry0, ry1);
    setup(2, bz0, bz1, rz0, rz1);

    i = p[bx0];
    j = p[bx1];

    b00 = p[i + by0];
    b10 = p[j + by0];
    b01 = p[i + by1];
    b11 = p[j + by1];
#define at(rx, ry, rz) (rx * q[0] + ry * q[1] + rz * q[2])

#define surve(t) (t * t * (3. - 2. * t))

#define lerp(t, a, b) (a + t * (b - a))

    sx = surve(rx0);
    sy = surve(ry0);
    sz = surve(rz0);

    q = g[b00 + bz0];
    u = at(rx0, ry0, rz0);
    q = g[b10 + bz0];
    v = at(rx1, ry0, rz0);
    a = lerp(sx, u, v);

    q = g[b01 + bz0];
    u = at(rx0, ry1, rz0);
    q = g[b11 + bz0];
    v = at(rx1, ry1, rz0);
    b = lerp(sx, u, v);

    c = lerp(sy, a, b); /* interpolate in y at lo x */

    q = g[b00 + bz1];
    u = at(rx0, ry0, rz1);
    q = g[b10 + bz1];
    v = at(rx1, ry0, rz1);
    a = lerp(sx, u, v);

    q = g[b01 + bz1];
    u = at(rx0, ry1, rz1);
    q = g[b11 + bz1];
    v = at(rx1, ry1, rz1);
    b = lerp(sx, u, v);

    d = lerp(sy, a, b); /* interpolate in y at hi x */

    return 1.5 * lerp(sz, c, d); /* interpolate in z */
}

static void init()
{
    int i, j, k;
    float v[3], s;

    /* Create an array of random gradient vectors uniformly on the unit sphere */

    srand(1);
    for (i = 0; i < B; i++) {
        do { /* Choose uniformly in a cube */
            for (j = 0; j < 3; j++)
                v[j] = (float)((rand() % (B + B)) - B) / B;
            s = DOT(v, v);
        } while (s > 1.0); /* If not in sphere try again */
        s = sqrt(s);
        for (j = 0; j < 3; j++) /* Else normalize */
            g[i][j] = v[j] / s;
    }

    /* Create a pseudorandom permutation of [1..B] */

    for (i = 0; i < B; i++)
        p[i] = i;
    for (i = B; i > 0; i -= 2) {
        k = p[i];
        p[i] = p[j = rand() % B];
        p[j] = k;
    }

    /* Extend g and p arrays to allow for faster indexing */

    for (i = 0; i < B + 2; i++) {
        p[B + i] = p[i];
        for (j = 0; j < 3; j++)
            g[B + i][j] = g[i][j];
    }
}

static float turbulence(float x, float y, float z, float lofreq, float hifreq)
{
    float freq, t, p[3];

    p[0] = x + 123.456;
    p[1] = y;
    p[2] = z;

    t = 0;
    for (freq = lofreq; freq < hifreq; freq *= 2.) {
        t += fabs(noise3(p)) / freq;
        p[0] *= 2.;
        p[1] *= 2.;
        p[2] *= 2.;
    }
    return t - 0.3; /* readjust to make mean value = 0.0 */
}

static float marble(float x, float y, float z)
{
    float m;
    m = turbulence(x, y, z, 0.3, 400.0);
    if (m > 1.0)
        m = 1.0;
    if (m < 0.0)
        m = 0.0;
    return m;
}

static uint8_t white_square[TXSX][TXSY][3];
static uint8_t black_square[TXSX][TXSY][3];
static uint8_t wood[TXSX][TXSY][3];

static ocf::Texture* whiteSquareTexture = nullptr;
static ocf::Texture* blackSquareTexture = nullptr;
static ocf::Texture* woodTexture = nullptr;

void genarateTexture()
{
    int i, j, k;
    float x, y, t, w, b;
    for (i = 0; i < TXSX; i++) {
        for (j = 0; j < TXSY; j++) {
            x = ((float)i) / 20.0;
            y = ((float)j) / 20.0;
            t = marble(x, y, 0.0);

            t = 0.2 + t;
            if (t > 1.0)
                t = 1.0;

            wood[i][j][0] = (0.6 * t) * 255;
            wood[i][j][1] = (0.4 * t) * 255;
            wood[i][j][2] = (0.5 - 0.4 * t) * 255;

            x = ((float)i) / 20.0;
            y = ((float)j) / 20.0;
            t = marble(x, y, 0.0);

            t = 0.2 + t;
            if (t > 1.0)
                t = 1.0;

            w = t;
            b = 0.8 - t;
            if (b < 0.0)
                b = 0.0;

            for (k = 0; k < 3; k++) {
                white_square[i][j][k] = w * 255;
                black_square[i][TXSY - j][k] = b * 255;
            }
        }
    }

    /* Create white texture */
    Texture::PixelBufferDescriptor white_square_buffer(white_square, sizeof(white_square), Texture::Format::RGB,
                                          Texture::Type::UNSIGNED_BYTE, nullptr);
    whiteSquareTexture =
        Texture::create(Texture::Sampler::SAMPLER_2D, TXSX, TXSY, 1, Texture::InternalFormat::RGB8);
    whiteSquareTexture->setImage(0, std::move(white_square_buffer));

    /* Create black texture */
    Texture::PixelBufferDescriptor black_square_buffer(black_square, sizeof(black_square), Texture::Format::RGB,
                                                       Texture::Type::UNSIGNED_BYTE, nullptr);
    blackSquareTexture = Texture::create(Texture::Sampler::SAMPLER_2D, TXSX, TXSY, 1, Texture::InternalFormat::RGB8);
    blackSquareTexture->setImage(0, std::move(black_square_buffer));


    /* Create wood texture */
    Texture::PixelBufferDescriptor wood_buffer(wood, sizeof(wood), Texture::Format::RGB,
                                               Texture::Type::UNSIGNED_BYTE, nullptr);
    woodTexture =
        Texture::create(Texture::Sampler::SAMPLER_2D, TXSX, TXSY, 1, Texture::InternalFormat::RGB8);
    woodTexture->setImage(0, std::move(wood_buffer));
}

ocf::Texture* getWhiteSquareTexture()
{
    return whiteSquareTexture;
}

ocf::Texture* getBlackSquareTexture()
{
    return blackSquareTexture;
}

ocf::Texture* getWoodTexture()
{
    return woodTexture;
}
