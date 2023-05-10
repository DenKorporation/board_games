using System.Net.Sockets;
using System.Text.Json;

namespace ServerLib;

class ClientObject
{
    protected internal string Id { get; } = Guid.NewGuid().ToString();
    protected internal StreamWriter Writer { get; }
    protected internal StreamReader Reader { get; }

    TcpClient client;
    ServerObject server;

    public ClientObject(TcpClient tcpClient, ServerObject serverObject)
    {
        client = tcpClient;
        server = serverObject;
        var stream = client.GetStream();
        Reader = new StreamReader(stream);
        Writer = new StreamWriter(stream);
    }

    public async Task ProcessAsync()
    {
        try
        {
            Console.WriteLine($"{Id}:Connection opened");

            string? message = await Reader.ReadLineAsync();
            using JsonDocument doc = JsonDocument.Parse(message);
            JsonElement element = doc.RootElement;
            
            switch (element.GetProperty("Type").ToString())
            {
                case "Create":
                    CreateRoomAction(element);
                    break;
                case "Connect":
                    break;
                case "List":
                    ListRoomAction();
                    break;
                case "Game":
                    break;
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.Message);
        }
        finally
        {
            server.RemoveConnection(Id);
        }
    }

    private void CreateRoomAction(JsonElement element)
    {
        server.AllGame.Add(new Game(element.GetProperty("Name").ToString()));
        Dictionary<string, string> reply = new()
        {
            { "Type", "Create" },
            { "Status", "Done" }
        };
            
        Writer.Write(JsonSerializer.Serialize(reply));
        Writer.Flush();
        
        Console.WriteLine($"{Id}:the room has been created");
    }

    private void ConnectRoomAction(JsonElement element)
    {
    }

    private void ListRoomAction()
    {
        string message = JsonSerializer.Serialize(server.AllGame);

        Writer.Write(message);
        Writer.Flush();

        Console.WriteLine($"{Id}:message sent");
    }

    private void GameAction(JsonElement element)
    {
    }


    protected internal void Close()
    {
        Console.WriteLine($"{Id}:connection closed");
        Writer.Close();
        Reader.Close();
        client.Close();
    }
}