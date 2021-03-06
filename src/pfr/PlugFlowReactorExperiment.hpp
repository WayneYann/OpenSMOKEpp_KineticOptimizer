/*-----------------------------------------------------------------------*\
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
|   Copyright(C) 2018  Alberto Cuoci                                      |
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

namespace OpenSMOKE
{
	void PlugFlowReactorExperiment::Setup(const std::string input_file_name,
		OpenSMOKE::ThermodynamicsMap_CHEMKIN*	thermodynamicsMapXML,
		OpenSMOKE::KineticsMap_CHEMKIN*			kineticsMapXML)
	{
		// Pointers
		thermodynamicsMapXML_ = thermodynamicsMapXML;
		kineticsMapXML_ = kineticsMapXML;

		// Defines the grammar rules
		OpenSMOKE::Grammar_PlugFlowReactorExperiment grammar_plugflowreactor;
		
		// Define the dictionaries
		std::string main_dictionary_name_ = "PlugFlowReactor";
		OpenSMOKE::OpenSMOKE_DictionaryManager dictionaries;
		dictionaries.ReadDictionariesFromFile(input_file_name);
		dictionaries(main_dictionary_name_).SetGrammar(grammar_plugflowreactor);

		// Read plug-flow reactor type
		{
			std::string value;
			if (dictionaries(main_dictionary_name_).CheckOption("@Type") == true)
			{
				dictionaries(main_dictionary_name_).ReadString("@Type", value);
				if (value == "Isothermal")	type_ = OpenSMOKE::PLUGFLOW_REACTOR_ISOTHERMAL;
				else if (value == "NonIsothermal")	type_ = OpenSMOKE::PLUGFLOW_REACTOR_NONISOTHERMAL;
				else OpenSMOKE::FatalErrorMessage("Unknown plug flow reactor type: " + value);

			}
		}

		bool constant_pressure = true;
		{
			if (dictionaries(main_dictionary_name_).CheckOption("@ConstantPressure") == true)
				dictionaries(main_dictionary_name_).ReadBool("@ConstantPressure", constant_pressure);
		}

		// Read initial conditions
		double T, P_Pa;
		OpenSMOKE::OpenSMOKEVectorDouble omega;
		{
			std::string name_of_gas_status_subdictionary;
			if (dictionaries(main_dictionary_name_).CheckOption("@InletStatus") == true)
				dictionaries(main_dictionary_name_).ReadDictionary("@InletStatus", name_of_gas_status_subdictionary);

			GetGasStatusFromDictionary(dictionaries(name_of_gas_status_subdictionary), *thermodynamicsMapXML, T, P_Pa, omega);
		}

		// Read integration limits
		end_value_ = 0.;
		bool time_independent_variable = false;

		// Read residence time
		{

			if (dictionaries(main_dictionary_name_).CheckOption("@ResidenceTime") == true)
			{
				double value;
				std::string units;
				dictionaries(main_dictionary_name_).ReadMeasure("@ResidenceTime", value, units);
				if (units == "s")		  end_value_ = value;
				else if (units == "ms")   end_value_ = value / 1000.;
				else if (units == "min")  end_value_ = value * 60.;
				else if (units == "h")    end_value_ = value * 3600.;
				else OpenSMOKE::FatalErrorMessage("Unknown time units");
				time_independent_variable = true;
			}
		}

		// Read length
		{
			if (dictionaries(main_dictionary_name_).CheckOption("@Length") == true)
			{
				double value;
				std::string units;
				dictionaries(main_dictionary_name_).ReadMeasure("@Length", value, units);
				if (units == "m")		  end_value_ = value;
				else if (units == "cm")   end_value_ = value / 100.;
				else if (units == "mm")   end_value_ = value / 1000.;
				else OpenSMOKE::FatalErrorMessage("Unknown length units");
				time_independent_variable = false;
			}
		}

		// Read diameter
		double diameter = 0.;
		{
			if (dictionaries(main_dictionary_name_).CheckOption("@Diameter") == true)
			{
				double value;
				std::string units;
				dictionaries(main_dictionary_name_).ReadMeasure("@Diameter", value, units);
				if (units == "m")		  diameter = value;
				else if (units == "cm")   diameter = value / 100.;
				else if (units == "mm")   diameter = value / 1000.;
				else OpenSMOKE::FatalErrorMessage("Unknown length units");
			}
		}

		// Read velocity
		double velocity = 0.;
		{
			if (dictionaries(main_dictionary_name_).CheckOption("@Velocity") == true)
			{
				double value;
				std::string units;
				dictionaries(main_dictionary_name_).ReadMeasure("@Velocity", value, units);
				if (units == "m/s")		   velocity = value;
				else if (units == "cm/s")  velocity = value / 1.e2;
				else if (units == "mm/s")  velocity = value / 1.e3;
				else OpenSMOKE::FatalErrorMessage("Unknown velocity units");
			}

			if (dictionaries(main_dictionary_name_).CheckOption("@VolumetricFlowRate") == true)
			{
				double value;
				std::string units;
				double volumetric_flow_rate;
				dictionaries(main_dictionary_name_).ReadMeasure("@VolumetricFlowRate", value, units);
				if (units == "m3/s")	    volumetric_flow_rate = value;
				else if (units == "cm3/s")  volumetric_flow_rate = value / 1.e6;
				else if (units == "mm3/s")  volumetric_flow_rate = value / 1.e9;
				else OpenSMOKE::FatalErrorMessage("Unknown volumetric flow rate units");

				const double A = PhysicalConstants::pi_over_4*diameter*diameter;
				velocity = volumetric_flow_rate / A;
			}

			if (dictionaries(main_dictionary_name_).CheckOption("@MassFlowRate") == true)
			{
				double value;
				std::string units;
				double mass_flow_rate;
				dictionaries(main_dictionary_name_).ReadMeasure("@MassFlowRate", value, units);
				if (units == "kg/s")	  mass_flow_rate = value;
				else if (units == "g/s")  mass_flow_rate = value / 1.e3;
				else OpenSMOKE::FatalErrorMessage("Unknown mass flow rate units");

				const double A = PhysicalConstants::pi_over_4*diameter*diameter;
				const double MW = thermodynamicsMapXML->MolecularWeight_From_MassFractions(omega.GetHandle());
				const double rho = P_Pa * MW / PhysicalConstants::R_J_kmol / T;
				velocity = mass_flow_rate / A / rho;
			}

			if (dictionaries(main_dictionary_name_).CheckOption("@MoleFlowRate") == true)
			{
				double value;
				std::string units;
				double mole_flow_rate;
				dictionaries(main_dictionary_name_).ReadMeasure("@MoleFlowRate", value, units);
				if (units == "kmol/s")		mole_flow_rate = value;
				else if (units == "mol/s")  mole_flow_rate = value / 1.e3;
				else OpenSMOKE::FatalErrorMessage("Unknown mole flow rate units");

				const double A = PhysicalConstants::pi_over_4*diameter*diameter;
				const double cTot = P_Pa / PhysicalConstants::R_J_kmol / T;
				velocity = mole_flow_rate / A / cTot;
			}
		}

		// Read ratio between cross section and perimeter
		double cross_section_over_perimeter = 0.;
		{
			double value;
			std::string units;
			if (dictionaries(main_dictionary_name_).CheckOption("@CrossSectionOverPerimeter") == true)
			{
				dictionaries(main_dictionary_name_).ReadMeasure("@CrossSectionOverPerimeter", value, units);
				if (units == "m")        cross_section_over_perimeter = value;
				else if (units == "dm")  cross_section_over_perimeter = value / 1.e1;
				else if (units == "cm")  cross_section_over_perimeter = value / 1.e2;
				else if (units == "mm")  cross_section_over_perimeter = value / 1.e3;
				else OpenSMOKE::FatalErrorMessage("Unknown length units");
			}
		}

		// Read global thermal exchange coefficient
		double global_thermal_exchange_coefficient = 0.;
		{
			double value;
			std::string units;
			if (dictionaries(main_dictionary_name_).CheckOption("@GlobalThermalExchangeCoefficient") == true)
			{
				dictionaries(main_dictionary_name_).ReadMeasure("@GlobalThermalExchangeCoefficient", value, units);
				if (units == "W/m2/K")			global_thermal_exchange_coefficient = value;
				else if (units == "W/m2/C")		global_thermal_exchange_coefficient = value;
				else if (units == "kcal/m2/K")		global_thermal_exchange_coefficient = value * 4186.8;
				else if (units == "kcal/m2/C")		global_thermal_exchange_coefficient = value * 4186.8;
				else OpenSMOKE::FatalErrorMessage("Unknown global thermal exchange coefficient units");
			}
		}

		// Environment temperature
		double T_environment = T;
		{
			double value;
			std::string units;
			if (dictionaries(main_dictionary_name_).CheckOption("@EnvironmentTemperature") == true)
			{
				dictionaries(main_dictionary_name_).ReadMeasure("@EnvironmentTemperature", value, units);
				if (units == "K")		T_environment = value;
				else if (units == "C")	T_environment = value + 273.15;
				else OpenSMOKE::FatalErrorMessage("Unknown temperature units");
			}
		}

		// Profile
		bool temperature_profile = false;
		OpenSMOKE::PlugFlowReactor_Profile*	profile;
		{
			std::string name_of_gas_status_subdictionary;
			if (dictionaries(main_dictionary_name_).CheckOption("@TemperatureProfile") == true)
			{
				if (type_ == OpenSMOKE::PLUGFLOW_REACTOR_NONISOTHERMAL)
					OpenSMOKE::FatalErrorMessage("The @TemperatureProfile cannot be used for adiabatic reactors");

				dictionaries(main_dictionary_name_).ReadDictionary("@TemperatureProfile", name_of_gas_status_subdictionary);

				OpenSMOKE::OpenSMOKEVectorDouble x, y;
				std::string x_variable, y_variable;
				GetXYProfileFromDictionary(dictionaries(name_of_gas_status_subdictionary), x, y, x_variable, y_variable);

				if (x_variable == "time" && time_independent_variable == false)
					OpenSMOKE::FatalErrorMessage("The @TemperatureProfile must be defined versus the time");
				if (x_variable == "length" && time_independent_variable == true)
					OpenSMOKE::FatalErrorMessage("The @TemperatureProfile must be defined versus the length");
				if (std::fabs(y[1] - T) > 1e-6)
					OpenSMOKE::FatalErrorMessage("Please check the @TemperatureProfile: the inlet values do not match");
				if (std::fabs(x[x.Size()] - end_value_) / end_value_ > 1.e-6)
					OpenSMOKE::FatalErrorMessage("Please check the @TemperatureProfile: the x domains do not match");

				temperature_profile = true;
				profile = new OpenSMOKE::PlugFlowReactor_Profile(x, y, x_variable);
			}
		}

		// Options
		{
			plugflow_options_ = new OpenSMOKE::PlugFlowReactor_Options();
			if (dictionaries(main_dictionary_name_).CheckOption("@Options") == true)
			{
				std::string name_of_options_subdictionary;
				dictionaries(main_dictionary_name_).ReadDictionary("@Options", name_of_options_subdictionary);
				plugflow_options_->SetupFromDictionary(dictionaries(name_of_options_subdictionary));
			}
		}

		// ODE Parameters
		{
			ode_parameters_ = new OpenSMOKE::ODE_Parameters();
			if (dictionaries(main_dictionary_name_).CheckOption("@OdeParameters") == true)
			{
				std::string name_of_ode_parameters_subdictionary;
				dictionaries(main_dictionary_name_).ReadDictionary("@OdeParameters", name_of_ode_parameters_subdictionary);
				ode_parameters_->SetupFromDictionary(dictionaries(name_of_ode_parameters_subdictionary));
			}
		}

		// Sensitivity Options
		{
			sensitivity_options_ = new OpenSMOKE::SensitivityAnalysis_Options();
			if (dictionaries(main_dictionary_name_).CheckOption("@SensitivityAnalysis") == true)
			{
				std::string name_of_sensitivity_options_subdictionary;
				dictionaries(main_dictionary_name_).ReadDictionary("@SensitivityAnalysis", name_of_sensitivity_options_subdictionary);

				plugflow_options_->SetSensitivityAnalysis(true);
				sensitivity_options_->SetupFromDictionary(dictionaries(name_of_sensitivity_options_subdictionary));
			}
		}

		// On the fly ROPA
		{
			onTheFlyROPA_ = new OpenSMOKE::OnTheFlyROPA(*thermodynamicsMapXML, *kineticsMapXML);
			if (dictionaries(main_dictionary_name_).CheckOption("@OnTheFlyROPA") == true)
			{
				std::string name_of_options_subdictionary;
				dictionaries(main_dictionary_name_).ReadDictionary("@OnTheFlyROPA", name_of_options_subdictionary);
				//	onTheFlyROPA_->SetupFromDictionary(dictionaries(name_of_options_subdictionary), path_kinetics_output);
			}
		}

		// On the fly PostProcessing
		{
			on_the_fly_post_processing_ = new OpenSMOKE::OnTheFlyPostProcessing(*thermodynamicsMapXML, *kineticsMapXML, plugflow_options_->output_path());

			if (dictionaries(main_dictionary_name_).CheckOption("@OnTheFlyPostProcessing") == true)
			{
				std::string name_of_options_subdictionary;
				dictionaries(main_dictionary_name_).ReadDictionary("@OnTheFlyPostProcessing", name_of_options_subdictionary);
				on_the_fly_post_processing_->SetupFromDictionary(dictionaries(name_of_options_subdictionary));
			}
		}

		// Ignition Delay Times
		{
			idt = new OpenSMOKE::IgnitionDelayTimes_Analyzer();
			if (dictionaries(main_dictionary_name_).CheckOption("@IgnitionDelayTimes") == true)
			{
				if (type_ == OpenSMOKE::PLUGFLOW_REACTOR_ISOTHERMAL)
					OpenSMOKE::FatalErrorMessage("The @IgnitionDelayTimes can be used only for NonIsothermal reactors");

				std::string name_of_idt_subdictionary;
				dictionaries(main_dictionary_name_).ReadDictionary("@IgnitionDelayTimes", name_of_idt_subdictionary);
				idt->SetupFromDictionary(dictionaries(name_of_idt_subdictionary), *thermodynamicsMapXML);
			}
		}

		// Polimi soot
		{
			polimi_soot_ = new OpenSMOKE::PolimiSoot_Analyzer(thermodynamicsMapXML);

			std::string name_of_polimisoot_analyzer_subdictionary;
			if (dictionaries(main_dictionary_name_).CheckOption("@PolimiSoot") == true)
			{
				dictionaries(main_dictionary_name_).ReadDictionary("@PolimiSoot", name_of_polimisoot_analyzer_subdictionary);
				polimi_soot_->SetupFromDictionary(dictionaries(name_of_polimisoot_analyzer_subdictionary));
			}
		}

		// Optimization
		{
			optimization_ = new OpenSMOKE::OptimizationRules_PlugFlowReactorExperiment;

			std::string name_of_optimization_subdictionary;
			if (dictionaries(main_dictionary_name_).CheckOption("@Optimization") == true)
			{
				dictionaries(main_dictionary_name_).ReadDictionary("@Optimization", name_of_optimization_subdictionary);
				optimization_->SetupFromDictionary(dictionaries(name_of_optimization_subdictionary), dictionaries);
			}
		}

		// ------------------------------------------------------------------------------------------- //
		//                              Non parametric analysis                                        //
		// ------------------------------------------------------------------------------------------- //
		// Solve the ODE system: NonIsothermal, Constant Volume
		if (type_ == OpenSMOKE::PLUGFLOW_REACTOR_ISOTHERMAL)
		{
			plugflow_isothermal_ = new OpenSMOKE::PlugFlowReactor_Isothermal(*thermodynamicsMapXML, *kineticsMapXML,
				*ode_parameters_, *plugflow_options_, *onTheFlyROPA_, *on_the_fly_post_processing_, *idt, *polimi_soot_, time_independent_variable, constant_pressure,
				velocity, T, P_Pa, omega);

			if (temperature_profile == true)
				plugflow_isothermal_->SetTemperatureProfile(*profile);
		}


		else if (type_ == OpenSMOKE::PLUGFLOW_REACTOR_NONISOTHERMAL)
		{
			plugflow_non_isothermal_ = new OpenSMOKE::PlugFlowReactor_NonIsothermal(*thermodynamicsMapXML, *kineticsMapXML,
				*ode_parameters_, *plugflow_options_, *onTheFlyROPA_, *on_the_fly_post_processing_, *idt, *polimi_soot_, time_independent_variable, constant_pressure,
				velocity, T, P_Pa, omega,
				global_thermal_exchange_coefficient, cross_section_over_perimeter, T_environment);
		}
	}

	void PlugFlowReactorExperiment::Solve(const bool verbose)
	{
		if (type_ == OpenSMOKE::PLUGFLOW_REACTOR_ISOTHERMAL)
		{
			plugflow_isothermal_->Solve(end_value_);
		}
		else if (type_ == OpenSMOKE::PLUGFLOW_REACTOR_NONISOTHERMAL)
		{
			plugflow_non_isothermal_->Solve(end_value_);
		}

		double tau = idt->temperature_slope_tau();
		if (optimization_->slope_definition() == false) 
			tau = idt->temperature_increase_tau();

		std::cout << tau << " " << optimization_->tau() << std::endl;

		const double abs_error = (optimization_->tau() - tau);
		const double rel_error = (optimization_->tau() - tau) / optimization_->tau();
		norm2_abs_error_ = abs_error * abs_error;
		norm2_rel_error_ = rel_error * rel_error;

		if (verbose == true)
		{
			std::cout << "Norm2(abs): " << norm2_abs_error_ << std::endl;
			std::cout << "Norm2(rel): " << norm2_rel_error_ << std::endl;
		}

		idt->Reset();
	}

	double PlugFlowReactorExperiment::Solve(const double x)
	{
		return 0;
	}

}
