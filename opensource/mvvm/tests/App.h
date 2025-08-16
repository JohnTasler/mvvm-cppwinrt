#pragma once
#include "App.xaml.g.h"

namespace winrt::mvvm::tests::implementation
{
    struct App : AppT<App>
    {
        App();
        void InitializeComponent();
        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(winrt::Windows::Foundation::IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
    };
}
