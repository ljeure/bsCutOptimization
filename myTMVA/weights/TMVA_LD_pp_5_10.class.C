// Class: ReadLD
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : LD::LD
TMVA Release   : 4.2.0         [262656]
ROOT Release   : 5.34/36       [336420]
Creator        : leure
Date           : Tue Jul 25 21:37:52 2017
Host           : Linux lcgapp-slc6-x86-64-23.cern.ch 2.6.32-573.18.1.el6.x86_64 #1 SMP Wed Feb 10 14:02:50 CET 2016 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /data/leure/bsTMVACDozen/myTMVA
Training events: 1209
Analysis type  : [Classification]


#OPT -*-*-*-*-*-*-*-*-*-*-*-*- options -*-*-*-*-*-*-*-*-*-*-*-*-

# Set by User:
V: "False" [Verbose output (short form of "VerbosityLevel" below - overrides the latter one)]
VarTransform: "None" [List of variable transformations performed before training, e.g., "D_Background,P_Signal,G,N_AllClasses" for: "Decorrelation, PCA-transformation, Gaussianisation, Normalisation, each for the given class of events ('AllClasses' denotes all events of all classes, if no class indication is given, 'All' is assumed)"]
H: "True" [Print method-specific help message]
CreateMVAPdfs: "True" [Create PDFs for classifier outputs (signal and background)]
# Default:
VerbosityLevel: "Default" [Verbosity level]
IgnoreNegWeightsInTraining: "False" [Events with negative weights are ignored in the training (but are included for testing and performance evaluation)]
##


#VAR -*-*-*-*-*-*-*-*-*-*-*-* variables *-*-*-*-*-*-*-*-*-*-*-*-

NVar 4
Bchi2cl                       Bchi2cl                       Bchi2cl                       Bchi2cl                                                         'F'    [0.0100242588669,0.999844372272]
BsvpvDistance/BsvpvDisErr     BsvpvDistance_D_BsvpvDisErr   BsvpvDistance/BsvpvDisErr     BsvpvDistance/BsvpvDisErr                                       'F'    [1.30181634426,1567.77709961]
Bd0/Bd0Err                    Bd0_D_Bd0Err                  Bd0/Bd0Err                    Bd0/Bd0Err                                                      'F'    [101.687400818,58774.5976562]
Blxy                          Blxy                          Blxy                          Blxy                                                            'F'    [0.0252494588494,1.48439598083]
NSpec 0


============================================================================ */

#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#ifndef IClassifierReader__def
#define IClassifierReader__def

class IClassifierReader {

 public:

   // constructor
   IClassifierReader() : fStatusIsClean( true ) {}
   virtual ~IClassifierReader() {}

   // return classifier response
   virtual double GetMvaValue( const std::vector<double>& inputValues ) const = 0;

   // returns classifier status
   bool IsStatusClean() const { return fStatusIsClean; }

 protected:

   bool fStatusIsClean;
};

#endif

class ReadLD : public IClassifierReader {

 public:

   // constructor
   ReadLD( std::vector<std::string>& theInputVars ) 
      : IClassifierReader(),
        fClassName( "ReadLD" ),
        fNvars( 4 ),
        fIsNormalised( false )
   {      
      // the training input variables
      const char* inputVars[] = { "Bchi2cl", "BsvpvDistance/BsvpvDisErr", "Bd0/Bd0Err", "Blxy" };

      // sanity checks
      if (theInputVars.size() <= 0) {
         std::cout << "Problem in class \"" << fClassName << "\": empty input vector" << std::endl;
         fStatusIsClean = false;
      }

      if (theInputVars.size() != fNvars) {
         std::cout << "Problem in class \"" << fClassName << "\": mismatch in number of input values: "
                   << theInputVars.size() << " != " << fNvars << std::endl;
         fStatusIsClean = false;
      }

      // validate input variables
      for (size_t ivar = 0; ivar < theInputVars.size(); ivar++) {
         if (theInputVars[ivar] != inputVars[ivar]) {
            std::cout << "Problem in class \"" << fClassName << "\": mismatch in input variable names" << std::endl
                      << " for variable [" << ivar << "]: " << theInputVars[ivar].c_str() << " != " << inputVars[ivar] << std::endl;
            fStatusIsClean = false;
         }
      }

      // initialize min and max vectors (for normalisation)
      fVmin[0] = 0;
      fVmax[0] = 0;
      fVmin[1] = 0;
      fVmax[1] = 0;
      fVmin[2] = 0;
      fVmax[2] = 0;
      fVmin[3] = 0;
      fVmax[3] = 0;

      // initialize input variable types
      fType[0] = 'F';
      fType[1] = 'F';
      fType[2] = 'F';
      fType[3] = 'F';

      // initialize constants
      Initialize();

   }

   // destructor
   virtual ~ReadLD() {
      Clear(); // method-specific
   }

   // the classifier response
   // "inputValues" is a vector of input values in the same order as the 
   // variables given to the constructor
   double GetMvaValue( const std::vector<double>& inputValues ) const;

 private:

   // method-specific destructor
   void Clear();

   // common member variables
   const char* fClassName;

   const size_t fNvars;
   size_t GetNvar()           const { return fNvars; }
   char   GetType( int ivar ) const { return fType[ivar]; }

   // normalisation of input variables
   const bool fIsNormalised;
   bool IsNormalised() const { return fIsNormalised; }
   double fVmin[4];
   double fVmax[4];
   double NormVariable( double x, double xmin, double xmax ) const {
      // normalise to output range: [-1, 1]
      return 2*(x - xmin)/(xmax - xmin) - 1.0;
   }

   // type of input variable: 'F' or 'I'
   char   fType[4];

   // initialize internal variables
   void Initialize();
   double GetMvaValue__( const std::vector<double>& inputValues ) const;

   // private members (method specific)
   std::vector<double> fLDCoefficients;
};

inline void ReadLD::Initialize() 
{
   fLDCoefficients.push_back( -0.136932384472 );
   fLDCoefficients.push_back( 0.348843962716 );
   fLDCoefficients.push_back( 0.000225010583305 );
   fLDCoefficients.push_back( 2.91982185237e-05 );
   fLDCoefficients.push_back( 0.139385247144 );

   // sanity check
   if (fLDCoefficients.size() != fNvars+1) {
      std::cout << "Problem in class \"" << fClassName << "\"::Initialize: mismatch in number of input values"
                << fLDCoefficients.size() << " != " << fNvars+1 << std::endl;
      fStatusIsClean = false;
   }         
}

inline double ReadLD::GetMvaValue__( const std::vector<double>& inputValues ) const
{
   double retval = fLDCoefficients[0];
   for (size_t ivar = 1; ivar < fNvars+1; ivar++) {
      retval += fLDCoefficients[ivar]*inputValues[ivar-1];
   }

   return retval;
}

// Clean up
inline void ReadLD::Clear() 
{
   // clear coefficients
   fLDCoefficients.clear(); 
}
   inline double ReadLD::GetMvaValue( const std::vector<double>& inputValues ) const
   {
      // classifier response value
      double retval = 0;

      // classifier response, sanity check first
      if (!IsStatusClean()) {
         std::cout << "Problem in class \"" << fClassName << "\": cannot return classifier response"
                   << " because status is dirty" << std::endl;
         retval = 0;
      }
      else {
         if (IsNormalised()) {
            // normalise variables
            std::vector<double> iV;
            iV.reserve(inputValues.size());
            int ivar = 0;
            for (std::vector<double>::const_iterator varIt = inputValues.begin();
                 varIt != inputValues.end(); varIt++, ivar++) {
               iV.push_back(NormVariable( *varIt, fVmin[ivar], fVmax[ivar] ));
            }
            retval = GetMvaValue__( iV );
         }
         else {
            retval = GetMvaValue__( inputValues );
         }
      }

      return retval;
   }
