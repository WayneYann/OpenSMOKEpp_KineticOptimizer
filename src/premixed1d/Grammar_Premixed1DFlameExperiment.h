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

#include "dictionary/OpenSMOKE_DictionaryManager.h"
#include "dictionary/OpenSMOKE_DictionaryGrammar.h"
#include "dictionary/OpenSMOKE_DictionaryKeyWord.h"

namespace OpenSMOKE
{
	class Grammar_Premixed1DFlameExperiment : public OpenSMOKE::OpenSMOKE_DictionaryGrammar
	{
	protected:

		virtual void DefineRules()
		{
			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@KineticsFolder",
				OpenSMOKE::SINGLE_PATH,
				"Name of the folder containing the kinetic scheme (XML Version)",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Backup",
				OpenSMOKE::SINGLE_PATH,
				"Name of backup file (XML Version)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@DontUseBackupGrid",
				OpenSMOKE::SINGLE_BOOL,
				"If true, the user defined grid is used, instead of the grid corresponding to the backup file (default:false)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Type",
				OpenSMOKE::SINGLE_STRING,
				"Type of simulation: BurnerStabilized | FlameSpeed",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Grid",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of the dictionary defining the mesh",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@InletStream",
				OpenSMOKE::VECTOR_STRING,
				"Name of the dictionary/dictionaries defining the inlet gas composition, temperature and pressure",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@OutletStream",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of the dictionary defining the outlet gas composition, temperature and pressure (this is only for initialization)",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@InletVelocity",
				OpenSMOKE::SINGLE_MEASURE,
				"Inlet velocity (i.e. flame speed): first guess",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@SensitivityAnalysis",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Dictionary containing additional options for solving the sensitivity analysis",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@DaeParameters",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Dictionary containing the numerical parameters for solving the stiff DAE system",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@NlsParameters",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Dictionary containing the numerical parameters for solving the NL system",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@FalseTransientParameters",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Dictionary containing the numerical parameters for solving the pseudo-transient phase",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Output",
				OpenSMOKE::SINGLE_PATH,
				"Name of the folder where to write the output files",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@UseNlsSolver",
				OpenSMOKE::SINGLE_BOOL,
				"Use the NLS solver to solve the steady state problems, after the DAE solver (default: true)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@UseDaeSolver",
				OpenSMOKE::SINGLE_BOOL,
				"Use the Dae solver (instead of NLS) to solve the steady state problems (default: true)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@PolimiSoot",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of the dictionary defining the rules for analyzing soot calculated using the Polimi mechanism",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@HMOM",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of the dictionary defining the rules for applying the Hybrid Method of Moments (HMOM)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@OnTheFlyPostProcessing",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Dictionary specifying the details for carrying out the post-processing analyses (on the fly)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@FixedTemperatureProfile",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of dictionary describing the temperature profile",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@FixedSpecificMassFlowRateProfile",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of dictionary describing the specific (i.e. per unit area) mass flow rate profile",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Soret",
				OpenSMOKE::SINGLE_BOOL,
				"Add Soret effect (default: true)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@SpeciesBundling",
				OpenSMOKE::SINGLE_DOUBLE,
				"Estimation of mass diffusion coefficients through the species bundling according the the specified maximum relative error (example: @SpeciesBundling 0.1)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Radiation",
				OpenSMOKE::SINGLE_BOOL,
				"Radiative heat transfer (default: none)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@EnvironmentTemperature",
				OpenSMOKE::SINGLE_MEASURE,
				"Environment temperature (default: 298.15 K)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@DerivativeGasMassFractions",
				OpenSMOKE::SINGLE_STRING,
				"Derivative gas mass fractions (default: backward)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@DerivativeGasTemperature",
				OpenSMOKE::SINGLE_STRING,
				"Derivative gas temperature (default: backward)",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@LewisNumbers",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of dictionary containing the list of Lewis numbers of species",
				false));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Optimization",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of the dictionary defining the optimization rules for applying the Virtual Chemistry (VC)",
				true));
		}
	};
}

