//*********************************************************
//
//    Copyright (c) John Tasler. All rights reserved.
//    This code is licensed under the MIT License.
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
//    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//    TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//    PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//*********************************************************
#pragma once
#ifndef __MVVM_CPPWINRT_PCH_H_INCLUDED
#define __MVVM_CPPWINRT_PCH_H_INCLUDED

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <string_view>
#include <type_traits>
#include <variant>

#include <wil/cppwinrt.h>

#if defined(GetCurrentTime)
#undef GetCurrentTime
#endif

#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>

#include <mvvm/delegate_command.h>
#include <mvvm/property_macros.h>
#include <mvvm/view.h>
#include <mvvm/view_model.h>

#include <wil/resource.h>

#endif // __MVVM_CPPWINRT_PCH_H_INCLUDED
