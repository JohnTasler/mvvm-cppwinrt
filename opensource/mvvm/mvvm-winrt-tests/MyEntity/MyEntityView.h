#pragma once

#include "MyEntityViewModel.h"
#include "MyEntityView.g.h"

namespace winrt::mvvm::tests::implementation
{
    struct MyEntityView
        : MyEntityViewT<MyEntityView>
        , ::mvvm::view<MyEntityView, MyEntityViewModel::class_type>
    {
        MyEntityView();
        MyEntityView(MyEntityViewModel::class_type const& viewModel);

        void ClickMeButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

    private:
        using view_base = typename ::mvvm::view<MyEntityView, MyEntityViewModel::class_type>;
    };
}

namespace winrt::mvvm::tests::factory_implementation
{
    struct MyEntityView
        : MyEntityViewT<MyEntityView, implementation::MyEntityView>
    {
    };
}
