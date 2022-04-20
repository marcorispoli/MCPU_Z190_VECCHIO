
#include "translator.h"
#include <QCoreApplication>

/**
 * @param deflang default language after declaration
 * in Integer format
 */
translatorClass::translatorClass(LANGUAGES deflang)
{
    // Inizializza alla lingua di default
    setLanguage(deflang);
}

/**
 * @param deflang default language after declaration
 * in String format
 */
translatorClass::translatorClass(QString deflang)
{
    // Inizializza alla lingua di default
    setLanguage(deflang);
}


// Controlla se la lingua è tra quelle pre-definite
/**
 * @param lng language to be verified in String format
 * @return integer code of the avilabl elanguage.
 * _LNG_UNDEF in caso di non disponibilità della lingua.
 */
unsigned char translatorClass::isLanguage(QString lng){

    if(lng == "ITA")        return _LNG_ITA;
    else if(lng == "DEU")   return _LNG_DEU;
    else if(lng == "FRA")   return _LNG_FRA;
    else if(lng == "ENG")   return _LNG_ENG;
    else if(lng == "PRT")   return _LNG_PRT;
    else if(lng == "RUS")   return _LNG_RUS;
    else if(lng == "ESP")   return _LNG_ESP;
    else if(lng == "TUR")   return _LNG_TUR;
    else if(lng == "POL")   return _LNG_POL;
    else if(lng == "CHN")   return _LNG_CHN;
    else if(lng == "LTU")   return _LNG_LTU;
    return _LNG_UNDEF;

}

/**
 * @param lng language code in String format
 */
void translatorClass::setLanguage(QString lng){
    setLanguage((LANGUAGES) isLanguage(lng));
}

/**
 * @param lng language code in Integer format
 */
void translatorClass::setLanguage(LANGUAGES lng)
{
    bool ris = false;

    switch(lng)
    {
     case _LNG_ITA:
        ris = traduttore.load("traduzione_ita.qm",":/Translate/Languages");
        break;
     case  _LNG_DEU:
        ris = traduttore.load("traduzione_ger.qm",":/Translate/Languages");
        break;
     case _LNG_FRA:
        ris = traduttore.load("traduzione_fra.qm",":/Translate/Languages");
        break;
      case _LNG_ENG:
        ris = traduttore.load("traduzione_eng.qm",":/Translate/Languages");
        break;
      case _LNG_PRT:
        ris = traduttore.load("traduzione_por.qm",":/Translate/Languages");
        break;
      case _LNG_RUS:
        ris = traduttore.load("traduzione_rus.qm",":/Translate/Languages");
        break;
      case _LNG_ESP:
        ris = traduttore.load("traduzione_spa.qm",":/Translate/Languages");
        break;
      case _LNG_TUR:
        ris = traduttore.load("traduzione_tur.qm",":/Translate/Languages");
        break;
      case _LNG_POL:
        ris = traduttore.load("traduzione_pol.qm",":/Translate/Languages");
       break;
      case _LNG_CHN:
        ris = traduttore.load("traduzione_chn.qm",":/Translate/Languages");
      break;
      case _LNG_LTU:
       ris = traduttore.load("traduzione_ltu.qm",":/Translate/Languages");
      break;
      case _LNG_UNDEF:
        ris = false;
        break;

    }

    if(ris){
        selectedLanguage = lng;
        QCoreApplication::installTranslator(&traduttore);
        emit changeLanguageSgn();
    }  else selectedLanguage = _LNG_UNDEF;

}



