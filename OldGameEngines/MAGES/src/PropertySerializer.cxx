/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include "MAGES/PropertySerializer.hxx"
using namespace MAGES;

///@todo Fix all these constructors so they can TRUTHFULLY guarantee to not throw exceptions
ReadException::ReadException() throw() {
	msg = String("ReadException: Encountered a read error.");
}


ReadException::ReadException(const String path) throw() {
	msg = String("ReadException: Encountered a read error on: ") + path;
}


ReadException::~ReadException() throw() { }


const char* ReadException::what() const throw() {
	return msg.c_str();
}


WriteException::WriteException() throw() {
	msg = String("WriteException: Encountered a write error.");
}


WriteException::WriteException(const String path) throw() {
	msg = String("WriteException: Encountered a write error on: ") + path;
}


WriteException::~WriteException() throw() { }


const char* WriteException::what() const throw() {
	return msg.c_str();
}
