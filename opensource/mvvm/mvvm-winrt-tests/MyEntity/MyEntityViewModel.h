#pragma once
#include "MyEntityViewModel.g.h"
#include <mvvm/delegate_command.h>
#include <mvvm/property_macros.h>
#include <mvvm/view_model.h>

namespace winrt::mvvm::tests::implementation
{
    struct MyEntityViewModel
        : MyEntityViewModelT<MyEntityViewModel>
        , ::mvvm::view_model<MyEntityViewModel>
    {
        MyEntityViewModel();

        int32_t MyProperty() { return get_property(m_myProperty); }
        void MyProperty(int32_t value) { set_property(m_myProperty, value, NAME_OF(MyEntityViewModel, MyProperty)); }

        DEFINE_PROPERTY_CALLBACK(bool, IsIncrementAvailable, true);
        DEFINE_PROPERTY_CALLBACK(bool, IsDecrementAvailable, true);

        winrt::Windows::UI::Xaml::Input::ICommand IncrementCommand();
        winrt::Windows::UI::Xaml::Input::ICommand DecrementCommand();
        winrt::Windows::UI::Xaml::Input::ICommand StringCommand();
        winrt::Windows::UI::Xaml::Input::ICommand Int32Command();

    private:
        int32_t m_myProperty{};
        winrt::com_ptr<::mvvm::delegate_command<void>> m_incrementCommand{ nullptr };
        winrt::com_ptr<::mvvm::delegate_command<IInspectable>> m_decrementCommand{ nullptr };
        winrt::com_ptr<::mvvm::delegate_command<hstring>> m_stringCommand{ nullptr };
        winrt::com_ptr<::mvvm::delegate_command<int32_t>> m_int32Command{ nullptr };
    };
}

namespace winrt::mvvm::tests::factory_implementation
{
    struct MyEntityViewModel
        : MyEntityViewModelT<MyEntityViewModel, implementation::MyEntityViewModel>
    {
    };
}
