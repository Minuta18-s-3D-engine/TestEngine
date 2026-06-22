#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
  : logger(Logging::createLogger("engine.assets.resourceManager")) {
    logger.info("ResourceManager initialized successfully.");
}
