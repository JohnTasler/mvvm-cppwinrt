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
        friend typename Derived;

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
        view_model() : view_model(nullptr)
        {
            // Default constructor is private to ensure that the view_model is always constructed with a dispatcher.
            // This prevents issues with UI thread access.
            static_assert(!std::is_same_v<Derived, view_model>, "Default constructor is not allowed for view_model.");
            static_assert(std::is_base_of_v<view_model<Derived>, Derived>, "Derived class must inherit from view_model");
        }

    protected:
        winrt::Windows::UI::Core::CoreDispatcher m_dispatcher{ nullptr };
    };
}

#endif // __MVVM_CPPWINRT_VIEW_MODEL_H_INCLUDED
