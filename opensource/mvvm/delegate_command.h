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
#ifndef __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED
#define __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED

#include <functional>
#include <type_traits>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#include <wil/com.h>
#include "property_macros.h"

namespace mvvm
{
    inline namespace
    {
        namespace wf = ::winrt::Windows::Foundation;
    }

    template <typename Parameter>
    struct delegate_command :
        winrt::implements<delegate_command<Parameter>, winrt::Windows::UI::Xaml::Input::ICommand>
    {
        // Types
        using NakedParameterType = std::conditional_t<std::is_same_v<Parameter, void>, void, std::remove_const_t<std::remove_reference_t<Parameter>>>;
        using ConstParameterType = std::conditional_t<std::is_same_v<Parameter, void>, void, std::add_const_t<NakedParameterType>>;
        using ExecuteHandler = std::function<void(std::add_lvalue_reference_t<ConstParameterType>)>;
        using CanExecuteHandler = std::function<bool(std::add_lvalue_reference_t<ConstParameterType>)>;

        // construction
        delegate_command() noexcept {}
        delegate_command(std::nullptr_t) noexcept {}

        delegate_command(ExecuteHandler&& executeHandler)
            : m_executeHandler(std::move(executeHandler))
        {
        }

        delegate_command(ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
            : m_executeHandler(std::move(executeHandler))
            , m_canExecuteHandler(std::move(canExecuteHandler))
        {
        }

        // ICommand interface methods
        DEFINE_EVENT_WITH_RAISE(wf::EventHandler<wf::IInspectable>, CanExecuteChanged, wf::IInspectable);
        bool CanExecute(wf::IInspectable const& parameter)
        {
            if (!m_canExecuteHandler)
            {
                return true;
            }
            else if (!m_executeHandler)
            {
                return false;
            }
            else if constexpr (std::is_same_v<Parameter, void>)
            {
                return std::invoke(m_canExecuteHandler);
            }
            else if constexpr (std::is_same_v<NakedParameterType, wf::IInspectable>)
            {
                return std::invoke(m_canExecuteHandler, parameter);
            }
            else if constexpr (std::is_convertible_v<NakedParameterType, wf::IInspectable>)
            {
                return std::invoke(m_canExecuteHandler, parameter.try_as<NakedParameterType>());
            }
            else
            {
                return std::invoke(m_canExecuteHandler, winrt::unbox_value_or<NakedParameterType>(parameter, {}));
            }
        }
        void Execute(wf::IInspectable const& parameter)
        {
            if constexpr (std::is_same_v<Parameter, void>)
            {
                return std::invoke(m_executeHandler);
            }
            else if constexpr (std::is_same_v<NakedParameterType, wf::IInspectable>)
            {
                std::invoke(m_executeHandler, parameter);
            }
            else if constexpr (std::is_convertible_v<NakedParameterType, wf::IInspectable>)
            {
                std::invoke(m_executeHandler, parameter.try_as<NakedParameterType>());
            }
            else
            {
                std::invoke(m_executeHandler, winrt::unbox_value_or<NakedParameterType>(parameter, {}));
            }
        }

    private:
        ExecuteHandler m_executeHandler;
        CanExecuteHandler m_canExecuteHandler;
    };
}

#endif // __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED
