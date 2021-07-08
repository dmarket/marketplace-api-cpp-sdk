.PHONY: setup
setup: ## Install all the build dependencies
ifeq ($(shell uname),Darwin)
	brew install cmake
else
	@echo 'please, visit https://cmake.org/install to install cmake'
endif

.PHONY: gen
gen: ## Generate client code
	cd api && cmake .

.DEFAULT_GOAL := gen

.PHONY: help
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'