//
// RelayCommand.cpp
// Implémentation de RelayCommand et des classes associées
//

#include "pch.h"
#include "RelayCommand.h"
#include "NavigationHelper.h"

using namespace WinRTControls::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Détermine si cette <see cref="RelayCommand"/> peut être exécutée dans son état actuel.
/// </summary>
/// <param name="parameter">
/// Données utilisées par la commande. Si la commande ne nécessite pas la transmission des données, cet objet peut avoir la valeur Null.
/// </param>
/// <returns>true si cette commande peut être exécutée ; sinon, false.</returns>
bool RelayCommand::CanExecute(Object^ parameter)
{
	return (_canExecuteCallback) (parameter);
}

/// <summary>
/// Exécute la commande <see cref="RelayCommand"/> sur la cible actuelle.
/// </summary>
/// <param name="parameter">
/// Données utilisées par la commande. Si la commande ne nécessite pas la transmission des données, cet objet peut avoir la valeur Null.
/// </param>
void RelayCommand::Execute(Object^ parameter)
{
	(_executeCallback) (parameter);
}

/// <summary>
/// Méthode utilisée pour déclencher l'événement <see cref="CanExecuteChanged"/>
/// pour indiquer que la valeur de retour de <see cref="CanExecute"/>
/// la méthode a changé.
/// </summary>
void RelayCommand::RaiseCanExecuteChanged()
{
	CanExecuteChanged(this, nullptr);
}

/// <summary>
/// Destructeur de classe RelayCommand.
/// </summary>
RelayCommand::~RelayCommand()
{
	_canExecuteCallback = nullptr;
	_executeCallback = nullptr;
};

/// <summary>
/// Crée une nouvelle commande qui peut toujours s'exécuter.
/// </summary>
/// <param name="canExecuteCallback">Logique d'état d'exécution.</param>
/// <param name="executeCallback">Logique d'exécution.</param>
RelayCommand::RelayCommand(std::function<bool(Platform::Object^)> canExecuteCallback,
	std::function<void(Platform::Object^)> executeCallback) :
	_canExecuteCallback(canExecuteCallback),
	_executeCallback(executeCallback)
	{
	}
