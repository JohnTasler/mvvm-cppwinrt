#pragma once
#include "MyEntityViewModel.g.h"

namespace winrt::mvvm::tests::implementation
{
    struct MyEntityViewModel
        : MyEntityViewModelT<MyEntityViewModel, Windows::UI::Xaml::Data::INotifyPropertyChanged>
        , ::mvvm::view_model<MyEntityViewModel>
    {
        MyEntityViewModel() = default;

        int32_t MyProperty() { return get_property(m_myProperty); }
        void MyProperty(int32_t value) { set_property(m_myProperty, value, NAME_OF(MyEntityViewModel, MyProperty)); }

    private:
        int32_t m_myProperty{};
    };
}

namespace winrt::mvvm::tests::factory_implementation
{
    struct MyEntityViewModel
        : MyEntityViewModelT<MyEntityViewModel, implementation::MyEntityViewModel>
    {
    };
}
