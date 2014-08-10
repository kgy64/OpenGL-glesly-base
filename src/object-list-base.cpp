/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Ducktor Navi - my Navigation Project
 * Purpose:     
 * Author:      Copyright (c) 2014 - Gyorgy Kovesdi <kgy@teledigit.eu>
 * License:     GPLv2
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "object-list-base.h"

#include <glesly/render.h>

using namespace Glesly;

float ObjectListBase::GetScreenAspect(void) const
{
 return GetRenderer().GetScreenAspect();
}

void ObjectListBase::KeyboardClick(UTF8::WChar ch)
{
 GetRenderer().KeyboardClick(ch);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
