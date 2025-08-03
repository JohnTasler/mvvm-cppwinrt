#include "pch.h"

#include "App.h"
#include "MyEntityView.h"

using namespace winrt;
using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::mvvm::tests;
using namespace winrt::mvvm::tests::implementation;

/// <summary>
/// Creates the singleton application object. This is the first line of authored code
/// executed, and as such is a logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
}

void App::InitializeComponent()
{
    Suspending({ this, &App::OnSuspending });

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& e)
{
    mvvm::tests::MyEntityView mainView{ nullptr };
    auto content = Window::Current().Content();
    if (content)
    {
        mainView = content.try_as<mvvm::tests::MyEntityView>();
    }

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (!mainView)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        mainView = winrt::make<MyEntityView>();

        if (e.PreviousExecutionState() == ApplicationExecutionState::Terminated)
        {
            // Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete
        }

        if (e.PrelaunchActivated() == false)
        {
            // Place the frame in the current Window
            Window::Current().Content(mainView);

            // Ensure the current window is active
            Window::Current().Activate();
        }
    }
    else
    {
        if (e.PrelaunchActivated() == false)
        {
            // Ensure the current window is active
            Window::Current().Activate();
        }
    }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    // Save application state and stop any background activity
}
