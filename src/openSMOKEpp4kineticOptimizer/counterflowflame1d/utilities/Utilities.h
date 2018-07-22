/*----------------------------------------------------------------------*\
|    ___                   ____  __  __  ___  _  _______                  |
|   / _ \ _ __   ___ _ __ / ___||  \/  |/ _ \| |/ / ____| _     _         |
|  | | | | '_ \ / _ \ '_ \\___ \| |\/| | | | | ' /|  _| _| |_ _| |_       |
|  | |_| | |_) |  __/ | | |___) | |  | | |_| | . \| |__|_   _|_   _|      |
|   \___/| .__/ \___|_| |_|____/|_|  |_|\___/|_|\_\_____||_|   |_|        |
|        |_|                                                              |
|                                                                         |
|   Author: Alberto Cuoci <alberto.cuoci@polimi.it>                       |
|   CRECK Modeling Group <http://creckmodeling.chem.polimi.it>            |
|   Department of Chemistry, Materials and Chemical Engineering           |
|   Politecnico di Milano                                                 |
|   P.zza Leonardo da Vinci 32, 20133 Milano                              |
|                                                                         |
|-------------------------------------------------------------------------|
|                                                                         |
|   This file is part of OpenSMOKE++ framework.                           |
|                                                                         |
|	License                                                               |
|                                                                         |
|   Copyright(C) 2014, 2013, 2012  Alberto Cuoci                          |
|   OpenSMOKE++ is free software: you can redistribute it and/or modify   |
|   it under the terms of the GNU General Public License as published by  |
|   the Free Software Foundation, either version 3 of the License, or     |
|   (at your option) any later version.                                   |
|                                                                         |
|   OpenSMOKE++ is distributed in the hope that it will be useful,        |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of        |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
|   GNU General Public License for more details.                          |
|                                                                         |
|   You should have received a copy of the GNU General Public License     |
|   along with OpenSMOKE++. If not, see <http://www.gnu.org/licenses/>.   |
|                                                                         |
\*-----------------------------------------------------------------------*/

#ifndef OpenSMOKE_CounterFlowFlame1D_Utilities_H
#define OpenSMOKE_CounterFlowFlame1D_Utilities_H

	/**
	*@brief Reads a solution from a backup file
	*@param path_file path to the backup file
	*@param x axial coordinate [m]
	*@param T temperatures [K]
	*@param P pressure profile [Pa]
	*@param U
	*@param G
	*@param H
	*@param omega species mass fractions profiles
	*@param names_species names of species
	*/
	void ReadFromBackupFile(const boost::filesystem::path path_file, std::vector<double>& x, std::vector<double>& T, std::vector<double>& P, 
							std::vector<double>& U, std::vector<double>& G, std::vector<double>& H, std::vector< std::vector<double> >& omega, std::vector<std::string>& names_species);

	/**
	*@brief Reads a solution from a backup file
	*@param path_file path to the backup file
	*@param thermodynamicsMap thermodynamic map from which names of species can be extracted
	*@param x axial coordinate [m]
	*@param T temperatures [K]
	*@param P pressure profile [Pa]
	*@param m mass flow rate profile [kg/m2/s]
	*@param omega species mass fractions profiles
	*/
	void ReadFromBackupFile(const boost::filesystem::path path_file, OpenSMOKE::ThermodynamicsMap_CHEMKIN& thermodynamicsMap,
							std::vector<double>& x, std::vector<double>& T, std::vector<double>& P, std::vector<double>& U, std::vector<double>& G, std::vector<double>& H, std::vector< std::vector<double> >& omega);


	double triangular_profile(const double y_fuel, const double y_oxidizer, const double y_peak, const double x_center, const double width_mixing, const double x)
	{
		if (x <= (x_center - 0.5*width_mixing))
		{
			return y_fuel;
		}
		else if (x > (x_center - 0.5*width_mixing) && x <= x_center)
		{
			return (y_peak - y_fuel) / (0.5*width_mixing)*(x - x_center + 0.50*width_mixing) + y_fuel;
		}
		else if ((x > x_center) && (x <= (x_center + 0.5*width_mixing)))
		{
			return (y_oxidizer - y_peak) / (0.5*width_mixing)*(x - x_center) + y_peak;
		}
		else
		{
			return y_oxidizer;
		}
	}

	double linear_profile(const double y_fuel, const double y_oxidizer, const double x_center, const double width_mixing, const double x)
	{
		if (x <= (x_center - 0.5*width_mixing))
		{
			return y_fuel;
		}
		else if (x < (x_center + 0.5*width_mixing))
		{
			return (y_oxidizer - y_fuel) / width_mixing*(x - x_center + 0.5*width_mixing) + y_fuel;
		}
		else
		{
			return y_oxidizer;
		}
	}

	double plateau_profile(const double y_fuel, const double y_oxidizer, const double y_peak, const double x_center, const double width_mixing, const double x_length, const double x)
	{
		if (x <= (x_center - 0.5*width_mixing))
		{
			return y_fuel + (y_peak - y_fuel)*x / (x_center - 0.5*width_mixing);
		}
		else if (x < (x_center + 0.5*width_mixing))
		{
			return y_peak;
		}
		else
		{
			return y_peak + (y_oxidizer - y_peak)*(x - (x_center + 0.5*width_mixing)) / (x_length - (x_center + 0.5*width_mixing));
		}
	}

#include "Utilities.hpp"

#endif	// OpenSMOKE_CounterFlowFlame1D_Utilities_H