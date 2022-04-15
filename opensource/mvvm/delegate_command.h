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
#ifndef __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED
#define __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED

#include <functional>
#include <type_traits>
#include <winrt/Windows.UI.Xaml.Input.h>

namespace mvvm
{
    template <typename Parameter>
    struct delegate_command
        : winrt::implements<delegate_command<Parameter>, winrt::Windows::UI::Xaml::Input::ICommand>
    {
    #pragma region constructors

        delegate_command() = default;

        template <typename ExecuteHandler>
        delegate_command(ExecuteHandler&& executeHandler)
            : m_executeHandler(std::move(executeHandler))
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        delegate_command(ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
            : m_executeHandler(std::move(executeHandler))
            , m_canExecuteHandler(std::move(canExecuteHandler))
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler, Parameter>);
        }

        template <typename ExecuteHandler>
        void Initialize(ExecuteHandler&& executeHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);

            if (m_executeHandler)
            {
                throw winrt::hresult_changed_state(L"Object has already been initialized: winrt::xaml::delegate_command");
            }

            m_executeHandler = std::move(executeHandler);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        void Initialize(ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler, Parameter>);

            this->Initialize<ExecuteHandler>(std::move(executeHandler));
            m_canExecuteHandler = std::move(canExecuteHandler);
        }

        template <typename ExecuteHandler>
        void Initialize(IInspectable const& callbackLifetimeObject, ExecuteHandler&& executeHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);

            if (callbackLifetimeObject)
            {
                m_callbackLifetimeObject = winrt::make_weak<IInspectable>(callbackLifetimeObject);
            }

            if (m_executeHandler)
            {
                throw winrt::hresult_changed_state(L"Object has already been initialized: winrt::xaml::delegate_command");
            }

            m_executeHandler = std::move(executeHandler);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        void Initialize(IInspectable const& callbackLifetimeObject, ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler, Parameter>);

            if (callbackLifetimeObject)
            {
                m_callbackLifetimeObject = winrt::make_weak<IInspectable>(callbackLifetimeObject);
            }

            this->Initialize<ExecuteHandler>(std::move(executeHandler));
            m_canExecuteHandler = std::move(canExecuteHandler);
        }

    #pragma endregion

    #pragma region ICommand

        winrt::event_token CanExecuteChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler)
        {
            return m_canExecuteChangedEventSource.add(handler);
        }

        void CanExecuteChanged(winrt::event_token token)
        {
            m_canExecuteChangedEventSource.remove(token);
        }

        bool CanExecuteHandler(winrt::Windows::Foundation::IInspectable const& parameter)
        {
            auto strongReference = m_callbackLifetimeObject.get();
            if (m_callbackLifetimeObject && !strongReference)
            {
                return false;
            }

            return CanExecutePrivate(parameter);
        }

        void ExecuteHandler(winrt::Windows::Foundation::IInspectable const& parameter)
        {
            auto strongReference = m_callbackLifetimeObject.get();
            if (m_callbackLifetimeObject && !strongReference)
            {
                return;
            }

            if (this->CanExecutePrivate(parameter))
            {
                if constexpr (std::is_same_v<Parameter, winrt::Windows::Foundation::IInspectable>)
                {
                    std::invoke(m_executeHandler, parameter);
                }
                else if constexpr (std::is_convertible_v<Parameter, winrt::Windows::Foundation::IInspectable>)
                {
                    std::invoke(m_executeHandler, parameter.try_as<Parameter>());
                }
                else
                {
                    std::invoke(m_executeHandler, winrt::unbox_value(parameter));
                }
            }
        }

    #pragma endregion

    #pragma region methods

        operator bool() { return m_executeHandler; }

        void raise_CanExecuteChanged()
        {
            if (m_canExecuteChangedEventSource)
            {
                m_canExecuteChangedEventSource(*this, winrt::Windows::Foundation::IInspectable{ nullptr });
            }
        }

    #pragma endregion

    #pragma region private implementation

    private:
        bool CanExecutePrivate(winrt::Windows::Foundation::IInspectable const& parameter)
        {
            if (!m_canExecuteHandler)
            {
                return true;
            }
            else if constexpr (std::is_same_v<Parameter, winrt::Windows::Foundation::IInspectable>)
            {
                return std::invoke(m_canExecuteHandler, parameter);
            }
            else if constexpr (std::is_convertible_v<Parameter, winrt::Windows::Foundation::IInspectable>)
            {
                return std::invoke(m_canExecuteHandler, parameter.try_as<Parameter>());
            }
            else
            {
                return std::invoke(m_canExecuteHandler, winrt::unbox_value(parameter));
            }
        }

    #pragma endregion

    #pragma region instance fields
    private:
        std::function<void(Parameter const&)> m_executeHandler;
        std::function<bool(Parameter const&)> m_canExecuteHandler;
        winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> m_canExecuteChangedEventSource;
        winrt::weak_ref<winrt::Windows::Foundation::IInspectable> m_callbackLifetimeObject;
    #pragma endregion
    };
}

#endif // __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED
