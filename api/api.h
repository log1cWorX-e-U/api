#pragma once

/// \file api.h
/// \brief Makros und Typedefs fuer Imports, Callbacks und Klassen in C.

#include <stdbool.h>
#include <stdint.h>

/// \brief Deklariert eine Funktion, die aus einer anderen Bibliothek importiert wird.
/// \param type Rueckgabetyp der Funktion.
/// \param function Funktionsname der Funktion.
/// \note Nur zur Deklaration in der *.h der importierenden Bibliothek verwenden.
#define protected_import(type, function) extern type function

/// \brief Platzhalter-Makro zur Kennzeichnung eines virtuellen Callbacks in der Anwendung.
/// \note Hat keine Parameter und expandiert zu nichts.
#define callback
/// \brief Deklariert einen schwachen (weak) Callback, den die Anwendung ueberschreiben kann.
/// \param type Rueckgabetyp des Callbacks.
/// \param function Funktionsname des Callbacks.
/// \note __attribute__((weak)) ist GCC/Clang-spezifisch.
#define callback_declaration(type, function) extern type function __attribute__((weak))

/// \brief Deklariert eine dateiprivate Funktion mit Unterstrich-Praefix.
/// \param type Rueckgabetyp der Funktion.
/// \param function Funktionsname; expandiert zu _<function>.
/// \note static — nur in der uebersetzenden Einheit sichtbar.
#define PRIVATE_FUNC(type, function) static type _##function

/// \brief Zeigertyp auf bool; _stack_t geprueft, _heap_t NULL-faehig.
typedef bool* const bool_stack_t, *bool_heap_t;
/// \brief Zeigertyp auf int16_t; _stack_t geprueft, _heap_t NULL-faehig.
typedef int16_t* const int16_stack_t, *int16_heap_t;
/// \brief Zeigertyp auf uint16_t; _stack_t geprueft, _heap_t NULL-faehig.
typedef uint16_t* const uint16_stack_t, *uint16_heap_t;
/// \brief Zeigertyp auf int32_t; _stack_t geprueft, _heap_t NULL-faehig.
typedef int32_t* const int32_stack_t, *int32_heap_t;
/// \brief Zeigertyp auf uint32_t; _stack_t geprueft, _heap_t NULL-faehig.
typedef uint32_t* const uint32_stack_t, *uint32_heap_t;
/// \brief Zeigertyp auf float; _stack_t geprueft, _heap_t NULL-faehig.
typedef float* const float_stack_t, *float_heap_t;
/// \brief Zeigertyp auf double; _stack_t geprueft, _heap_t NULL-faehig.
typedef double* const double_stack_t, *double_heap_t;

// Aus einem x_heap_t Pointer kann nach NULL-Prüfung ein x_stack_t werden!
//
// _stack_t bedeutet: der Zeiger ist gueltig, ein NULL-Test ist nicht noetig. Es gibt zwei Sorten:
//
//  1. Struct-Zeiger (meist auf dem Stack des Aufrufers): typedef struct foo {...}* const foo_stack_t;
//     Der const ist das Merkmal dieser Sorte — der Konsument schreibt nicht hindurch.
//  2. Funktionszeiger (in CLASS): CLASS_METHOD_PTR_DECL erzeugt class_foo_bar_stack_t. Hier ist
//     NICHTS const; die Zusage ist, dass new() jeden Methodenzeiger setzt (new() macht zwei Dinge:
//     malloc(sizeof(struct class_foo)) und das Initialisieren der Funktionszeiger). Ein solches
//     Objekt darf seine Methoden also ohne vorherige Pruefung aufrufen.
//
// NULL-faehige Rueckgaben sind immer x_heap_t (nie x_stack_t), NULL-faehige Member sind
// struct foo* — die _stack_t-Zusage gilt nur fuer gepruefte Zeiger.

/// \brief Beginnt eine Klassendefinition: erzeugt den Heap-Zeigertyp und den Struct-Kopf.
/// \param class_name Name der Klasse.
/// \note Die Expansion endet offen; der Struct-Rumpf folgt direkt darunter.
#define CLASS(class_name) typedef struct class_##class_name* class_##class_name##_heap_t;\
					struct class_##class_name

/// \brief Erzeugt den Funktionszeiger-Typ einer Klassenmethode.
/// \param class_name Name der Klasse.
/// \param type Rueckgabetyp der Methode.
/// \param function Methodenname.
/// \param ... Parameterliste der Methode (optional).
/// \note Der zugehoerige Zeigertyp wird hier erzeugt; die Standardimplementierung meldet die
///       .c-Datei selbst per PRIVATE_FUNC an (eine static-Deklaration im Header wuerde bei zwei
///       Klassen mit gleichnamiger Methode kollidieren).
#define CLASS_METHOD_PTR_DECL(class_name, type, function, ...)	typedef type (*class_##class_name##_##function##_fptr_t)(__VA_ARGS__)

/// \brief Deklariert die Methode einer Klasse.
/// \param class_name Name der Klasse.
/// \param type Rueckgabetyp der Methode.
/// \param function Methodenname; expandiert zu class_<class_name>_<function>.
#define CLASS_METHOD(class_name, type, function) type class_##class_name##_##function
