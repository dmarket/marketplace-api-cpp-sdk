## marketplace api cpp sdk

A tiny sdk for marketplace api cpp grpc client generation.

## quick start 

To setup local development environment run:

```shell script
make setup
```

To get list of all available commands run:

```shell script
make help
```

After applying changes to `api/marketplace/marketplace.proto` run:

```shell script
make gen
```

This will generate cpp grpc client with required deps.
