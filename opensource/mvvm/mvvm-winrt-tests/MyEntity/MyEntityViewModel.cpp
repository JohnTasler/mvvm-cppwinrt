#include "pch.h"
#include "MyEntityViewModel.h"
#if __has_include("MyEntityViewModel.g.cpp")
#include "MyEntityViewModel.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Input;
using namespace winrt::mvvm::tests::implementation;

ICommand MyEntityViewModel::IncrementCommand()
{
    if (!m_incrementCommand)
    {
        m_incrementCommand.Initialize(*this,
            [this] { MyProperty(MyProperty() + 1); },
            [this] { return IsIncrementAvailable(); });
    }

    return m_incrementCommand;
}

ICommand MyEntityViewModel::DecrementCommand()
{
    if (!m_decrementCommand)
    {
        m_decrementCommand.Initialize(*this,
            [this] { MyProperty(MyProperty() - 1); },
            [this] { return IsDecrementAvailable(); });
    }

    return m_decrementCommand;
}

void MyEntityViewModel::OnIsIncrementAvailableChanged([[maybe_unused]] bool const& oldValue, [[maybe_unused]] bool const& newValue)
{
    m_incrementCommand.raise_CanExecuteChanged();
}

void MyEntityViewModel::OnIsDecrementAvailableChanged([[maybe_unused]] bool const& oldValue, [[maybe_unused]] bool const& newValue)
{
    m_decrementCommand.raise_CanExecuteChanged();
}
