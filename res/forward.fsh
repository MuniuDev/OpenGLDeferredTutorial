/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#version 330 core
out vec4 color;

in vec2 texCoord;
uniform sampler2D gSampler;


void main()
{
    color = texture(gSampler, texCoord);
}
