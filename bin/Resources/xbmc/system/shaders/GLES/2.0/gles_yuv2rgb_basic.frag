/*
 *      Copyright (C) 2010-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#version 100

precision mediump float;

uniform sampler2D m_sampY;
uniform sampler2D m_sampU;
uniform sampler2D m_sampV;
varying vec2      m_cordY;
varying vec2      m_cordU;
varying vec2      m_cordV;
uniform vec2      m_step;
uniform mat4      m_yuvmat;
uniform float     m_alpha;

void main()
{
  vec4 rgb;
#if defined(XBMC_YV12) || defined(XBMC_NV12)

  vec4 yuv;
  yuv.rgba = vec4( texture2D(m_sampY, m_cordY).r
                 , texture2D(m_sampU, m_cordU).g
                 , texture2D(m_sampV, m_cordV).a
                 , 1.0 );

  rgb   = m_yuvmat * yuv;

  rgb.a = m_alpha;

#elif defined(XBMC_NV12_RRG)

  vec4 yuv;
  yuv.rgba = vec4( texture2D(m_sampY, m_cordY).r
                 , texture2D(m_sampU, m_cordU).r
                 , texture2D(m_sampV, m_cordV).g
                 , 1.0 );

  rgb   = m_yuvmat * yuv;

  rgb.a = m_alpha;

#elif defined(XBMC_YUY2) || defined(XBMC_UYVY)

  vec2 stepxy = m_step;
  vec2 pos    = m_cordY;
  pos         = vec2(pos.x - stepxy.x * 0.25, pos.y);
  vec2 f      = fract(pos / stepxy);

  //y axis will be correctly interpolated by opengl
  //x axis will not, so we grab two pixels at the center of two columns and interpolate ourselves
  vec4 c1 = texture2D(m_sampY, vec2(pos.x + (0.5 - f.x) * stepxy.x, pos.y));
  vec4 c2 = texture2D(m_sampY, vec2(pos.x + (1.5 - f.x) * stepxy.x, pos.y));

  /* each pixel has two Y subpixels and one UV subpixel
     YUV  Y  YUV
     check if we're left or right of the middle Y subpixel and interpolate accordingly*/
#ifdef XBMC_YUY2 //BGRA = YUYV
  float leftY   = mix(c1.b, c1.r, f.x * 2.0);
  float rightY  = mix(c1.r, c2.b, f.x * 2.0 - 1.0);
  vec2  outUV   = mix(c1.ga, c2.ga, f.x);
#else //BGRA = UYVY
  float leftY   = mix(c1.g, c1.a, f.x * 2.0);
  float rightY  = mix(c1.a, c2.g, f.x * 2.0 - 1.0);
  vec2  outUV   = mix(c1.br, c2.br, f.x);
#endif //XBMC_YUY2

  float outY    = mix(leftY, rightY, step(0.5, f.x));

  vec4  yuv     = vec4(outY, outUV, 1.0);
  rgb           = m_yuvmat * yuv;

  rgb.a = m_alpha;
#endif

  gl_FragColor = rgb;
}

