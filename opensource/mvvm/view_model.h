//*********************************************************
//
//    Copyright (c) Microsoft. All rights reserved.
//    This code is licensed under the MIT License.
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
//    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//    TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//    PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//*********************************************************
#pragma once
#ifndef __MVVM_CPPWINRT_VIEW_MODEL_H_INCLUDED
#define __MVVM_CPPWINRT_VIEW_MODEL_H_INCLUDED

#include "view_model_base.h"

namespace mvvm
{
    template <typename Derived>
    struct __declspec(empty_bases) view_model : view_model_base<Derived>
    {
        view_model() : view_model(nullptr)
        {
        }

        view_model(winrt::Windows::UI::Core::CoreDispatcher const& dispatcher)
        {
            if (dispatcher)
            {
                m_dispatcher = dispatcher;
            }
            else
            {
                m_dispatcher = winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread().Dispatcher();
                if (!m_dispatcher)
                {
                    throw winrt::hresult_wrong_thread(L"ViewModels must be instantiated on a UI thread."sv);
                }
            }
        }

        winrt::Windows::UI::Core::CoreDispatcher Dispatcher() const { return m_dispatcher; }

        winrt::Windows::UI::Core::CoreDispatcher get_dispatcher_override() { return m_dispatcher; }
    private:
        winrt::Windows::UI::Core::CoreDispatcher m_dispatcher{ nullptr };
    };
}

#endif // __MVVM_CPPWINRT_VIEW_MODEL_H_INCLUDED
