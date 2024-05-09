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

namespace juce
{

Desktop::Desktop()
{
}

Desktop::~Desktop()
{
    clearSingletonInstance();
}

int Desktop::getNumDisplays() const
{
    return displays.size();
}

Display* Desktop::getDisplay (int displayIndex) const
{
    if (isPositiveAndBelow (displayIndex, displays.size()))
        return displays.getUnchecked (displayIndex);

    return nullptr;
}

Display* Desktop::getPrimaryDisplay() const
{
    return !displays.isEmpty() ? getDisplay(0) : nullptr;
}

JUCE_IMPLEMENT_SINGLETON (Desktop)

} // namespace juce