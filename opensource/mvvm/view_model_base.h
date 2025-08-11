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
#ifndef __MVVM_CPPWINRT_VIEW_MODEL_BASE_H_INCLUDED
#define __MVVM_CPPWINRT_VIEW_MODEL_BASE_H_INCLUDED

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include "notify_property_changed.h"

namespace mvvm
{
    template <typename Derived>
    struct __declspec(empty_bases) view_model_base : notify_property_changed<Derived>
    {
        friend typename Derived;

        template <typename TValue>
        inline TValue get_property_override(TValue const& valueField)
        {
            if (this->derived().has_thread_access())
            {
                return base::notify_property_changed::get_property_core(valueField);
            }

            return [this, &valueField]() -> winrt::Windows::Foundation::IAsyncOperation<TValue>
            {
                co_await this->derived().Dispatcher();
                co_return base::notify_property_changed::get_property_core(valueField);
            }().get();
        }

        template <typename TValue, typename TOldValue, bool compare, typename propertyNameType>
        inline bool set_property_override(TValue& valueField, TValue const& newValue, TOldValue& oldValue, propertyNameType const& propertyNameOrNames)
        {
            if (this->derived().has_thread_access())
            {
                return this->set_property_core<TValue, TOldValue, compare, propertyNameType>(std::forward<TValue&>(valueField), newValue, oldValue, propertyNameOrNames);
            }

            auto lambda = [this, &valueField, &newValue, &oldValue, &propertyNameOrNames]() -> winrt::Windows::Foundation::IAsyncOperation<bool>
            {
                co_await winrt::resume_foreground(this->derived().get_dispatcher_override());
                co_return this->set_property_core<TValue, TOldValue, compare, propertyNameType>(std::forward<TValue&>(valueField), newValue, oldValue, propertyNameOrNames);
            };

            return lambda().get();
        }

        winrt::Windows::UI::Core::CoreDispatcher get_dispatcher_override() { return { nullptr }; }

        bool has_thread_access() const
        {
            auto dispatcher = this->derived().Dispatcher();
            return dispatcher && dispatcher.HasThreadAccess();
        }

        void check_thread() const
        {
            if (!has_thread_access())
            {
                throw winrt::hresult_wrong_thread();
            }
        }

    protected:
        // This is used to ensure that the derived class is actually derived from view_model_base
        view_model_base()
        {
            static_assert(std::is_base_of_v<view_model_base, Derived>, "Derived class must inherit from view_model_base");
        }
        using base = typename ::mvvm::notify_property_changed<Derived>;
    };
}

#endif // __MVVM_CPPWINRT_VIEW_MODEL_BASE_H_INCLUDED
