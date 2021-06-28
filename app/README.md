# Drogui App

## Comunication protocol

| **Data** | **Bytes** | **Description** |
|----|----|----|
|d$ | 2 Byte | Drogui message header|
|a<sup>[1](#actions)</sup> | 1 Byte | Action byte|
|pyld| n<sup>[1](#actions)</sup> Byte | Depending on action|

### Actions
| **Character**| **#N Bytes** | **Payload**|
|----|----|----|
| 'r'| 1 Byte | Register[1]|
| 'w'| 5 Bytes | Register[1] + Value[4] |
| 'c'| 1 Byte | Command id<sup>[2](#commands)</sup>[1]|

#### Commands
| **Character**| **Description**|
|----|----|
| 'l'| Start logging|
| 'n'| Stop logging|
