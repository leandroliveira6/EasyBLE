#include "EasyBLEServerCallback.hpp"
#include "EasyBLE.hpp"

/**
 * @brief Método callback acionado a cada nova conexão bluetooth.
 *
 * Método callback que atualiza o estado do servidor bluetooth para conectado quando uma conexão é estabelecida.
 */
void EasyBLEServerCallback::onConnect(BLEServer *pServer)
{
    EasyBLE::changeConnection(true);
};

/**
 * @brief Método callback acionado a cada nova desconexão bluetooth.
 *
 * Método callback que atualiza o estado do servidor bluetooth para desconectado quando uma conexão é perdida.
 */
void EasyBLEServerCallback::onDisconnect(BLEServer *pServer)
{
    EasyBLE::changeConnection(false);
}