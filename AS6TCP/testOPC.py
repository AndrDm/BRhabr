import asyncio
from asyncua import Client

# Server connection details
url = "opc.tcp://127.0.0.1:4840"
username = "User"
password = "user"
node_id = "ns=5;s=::Program:counterOPC"

async def read_opc_value(client, node):
    value = await node.read_value()
    print(f"Value of {node_id}: {value}")

async def main():
    # Create client and set user identity
    async with Client(url=url) as client:
        try:
            client.set_user(username)
            client.set_password(password)
            # await client.activate_session(username, password, None)
            await client.connect()
            print("Connected to OPC UA server")

            # Get the node
            node = client.get_node(node_id)

            # Read the value cyclically
            while True:
                try:
                    await read_opc_value(client, node)
                    await asyncio.sleep(1)
                except asyncio.CancelledError:
                    print("Stopping the cyclic read...")
                    break
                except Exception as e:
                    print(f"An error occurred during read: {e}")
                    await asyncio.sleep(1)

        except Exception as e:
            print(f"An error occurred: {e}")

if __name__ == "__main__":
    asyncio.run(main())
