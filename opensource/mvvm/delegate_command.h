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
#include <variant>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#include <wil/com.h>
#include "property_macros.h"

namespace mvvm
{
    struct __declspec(empty_bases) delegate_command_base
        : winrt::implements<delegate_command_base, winrt::Windows::UI::Xaml::Input::ICommand>
    {
    #if defined(_DEBUG) && !defined(WINRT_NO_MAKE_DETECTION)
        void use_make_function_to_create_this_object() override {}
    #endif

    #pragma region ICommand

        DEFINE_EVENT(winrt::Windows::Foundation::EventHandler<IInspectable>, CanExecuteChanged);
        virtual bool CanExecute(IInspectable const& parameter) = 0;
        virtual void Execute(IInspectable const& parameter) = 0;

    #pragma endregion

        void raise_CanExecuteChanged()
        {
            if (m_eventCanExecuteChanged)
            {
                m_eventCanExecuteChanged(*this, IInspectable{ nullptr });
            }
        }

        static void final_release(std::unique_ptr<delegate_command_base> ptr) noexcept
        {
            if (ptr->m_ownerObject)
            {
                ptr->m_ownerObject = nullptr;
                ptr.release();
            }
        }

    protected:
        wil::com_ptr<::IUnknown> m_ownerObject;
    };

    template <typename Parameter>
    struct delegate_command : delegate_command_base
    {
    #pragma region construction

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
                throw winrt::hresult_changed_state(L"Object has already been initialized: mvvm::delegate_command");
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
        void Initialize(IInspectable const& ownerObject, ExecuteHandler&& executeHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);

            m_ownerObject = winrt::get_unknown(ownerObject);
            this->Initialize(executeHandler);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        void Initialize(IInspectable const& ownerObject, ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler, Parameter>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler, Parameter>);

            this->Initialize<ExecuteHandler>(ownerObject, std::move(executeHandler));
            m_canExecuteHandler = std::move(canExecuteHandler);
        }

    #pragma endregion

    #pragma region ICommand
        bool CanExecute(winrt::Windows::Foundation::IInspectable const& parameter) override
        {
            if (!m_canExecuteHandler)
            {
                return true;
            }
            else if (!m_executeHandler)
            {
                return false;
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

        void Execute(winrt::Windows::Foundation::IInspectable const& parameter) override
        {
            if (this->CanExecute(parameter))
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

        operator bool() { return (bool)m_executeHandler; }

    #pragma region instance fields
    private:
        std::function<void(Parameter const&)> m_executeHandler;
        std::function<bool(Parameter const&)> m_canExecuteHandler;
    #pragma endregion
    };

    template <>
    struct delegate_command<void> : delegate_command_base
    {
    #pragma region construction

        delegate_command() = default;

        template <typename ExecuteHandler>
        delegate_command(ExecuteHandler&& executeHandler)
            : m_executeHandler(std::move(executeHandler))
        {
            static_assert(std::is_invocable_v<ExecuteHandler>);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        delegate_command(ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
            : m_executeHandler(std::move(executeHandler))
            , m_canExecuteHandler(std::move(canExecuteHandler))
        {
            static_assert(std::is_invocable_v<ExecuteHandler>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler>);
        }

        template <typename ExecuteHandler>
        void Initialize(ExecuteHandler&& executeHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler>);

            if (m_executeHandler)
            {
                throw winrt::hresult_changed_state(L"Object has already been initialized: mvvm::delegate_command");
            }

            m_executeHandler = std::move(executeHandler);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        void Initialize(ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler>);

            this->Initialize<ExecuteHandler>(std::move(executeHandler));
            m_canExecuteHandler = std::move(canExecuteHandler);
        }

        template <typename ExecuteHandler>
        void Initialize(IInspectable const& ownerObject, ExecuteHandler&& executeHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler>);

            m_ownerObject = winrt::get_unknown(ownerObject);
            this->Initialize(executeHandler);
        }

        template <typename ExecuteHandler, typename CanExecuteHandler>
        void Initialize(IInspectable const& ownerObject, ExecuteHandler&& executeHandler, CanExecuteHandler&& canExecuteHandler)
        {
            static_assert(std::is_invocable_v<ExecuteHandler>);
            static_assert(std::is_invocable_r_v<bool, CanExecuteHandler>);

            this->Initialize<ExecuteHandler>(ownerObject, std::move(executeHandler));
            m_canExecuteHandler = std::move(canExecuteHandler);
        }

    #pragma endregion

    #pragma region ICommand
        bool CanExecute([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& parameter) override
        {
            if (!m_canExecuteHandler)
            {
                return true;
            }
            else if (!m_executeHandler)
            {
                return false;
            }
            else
            {
                return std::invoke(m_canExecuteHandler);
            }
        }

        void Execute([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& parameter) override
        {
            if (this->CanExecute(parameter))
            {
                std::invoke(m_executeHandler);
            }
        }
    #pragma endregion

        operator bool() { return (bool)m_executeHandler; }

    #pragma region instance fields
    private:
        std::function<void()> m_executeHandler;
        std::function<bool()> m_canExecuteHandler;
    #pragma endregion
    };
}

#endif // __MVVM_CPPWINRT_DELEGATE_COMMAND_H_INCLUDED
