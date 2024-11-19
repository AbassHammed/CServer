#include "errors.h"

const char *get_error_message(int error_code)
{
    switch (error_code)
    {
    case ERROR_404:
        return "404 Non Trouvé";
    case ERROR_500:
        return "500 Erreur Interne du Serveur";
    case ERROR_403:
        return "403 Accès Refusé";
    case ERROR_405:
        return "405 Méthode Non Autorisée";
    default:
        return "Erreur Inconnue";
    }
}
