#include "pch.h"
#include "MyEntityViewModel.h"
#if __has_include("MyEntityViewModel.g.cpp")
#include "MyEntityViewModel.g.cpp"
#endif

#include <format>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Input;
using namespace winrt::mvvm::tests::implementation;

MyEntityViewModel::MyEntityViewModel()
{
    m_incrementCommand = winrt::make_self<::mvvm::delegate_command<void>>(
        [this]() { MyProperty(MyProperty() + 1); },
        [this]() { return IsIncrementAvailable(); } );
    m_decrementCommand = winrt::make_self<::mvvm::delegate_command<IInspectable>>(
        [this](auto) { MyProperty(MyProperty() - 1); },
        [this](auto) { return IsDecrementAvailable(); });
    m_stringCommand = winrt::make_self <::mvvm::delegate_command<hstring>>(
        [this](auto value) { OutputDebugStringW(std::format(L"m_stringCommand.Execute: value=\"{}\"\n", value).c_str()); },
        [this](auto value) { OutputDebugStringW(std::format(L"m_stringCommand.CanExecute: value=\"{}\"\n", value).c_str()); return true; });
    m_int32Command = winrt::make_self<::mvvm::delegate_command<int32_t>>(
        [this](auto value) { OutputDebugStringW(std::format(L"m_uintCommand.Execute: value={}\n", value).c_str()); },
        [this](auto value) { OutputDebugStringW(std::format(L"m_uintCommand.CanExecute: value={}\n", value).c_str()); return true; });
}

ICommand MyEntityViewModel::IncrementCommand()
{
    return *m_incrementCommand;
}

ICommand MyEntityViewModel::DecrementCommand()
{
    return *m_decrementCommand;
}

ICommand MyEntityViewModel::StringCommand()
{
    return *m_stringCommand;
}

ICommand MyEntityViewModel::Int32Command()
{
    return *m_int32Command;
}

void MyEntityViewModel::OnIsIncrementAvailableChanged([[maybe_unused]] bool const& oldValue, [[maybe_unused]] bool const& newValue)
{
    if (m_incrementCommand)
    {
        m_incrementCommand->raise_CanExecuteChanged();
    }
}

void MyEntityViewModel::OnIsDecrementAvailableChanged([[maybe_unused]] bool const& oldValue, [[maybe_unused]] bool const& newValue)
{
    if (m_decrementCommand)
    {
        m_decrementCommand->raise_CanExecuteChanged();
    }
}
