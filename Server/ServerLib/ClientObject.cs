using System.Net.Sockets;
using System.Text.Json;

namespace ServerLib;

class ClientObject
{
    internal string Id { get; } = Guid.NewGuid().ToString();
    internal StreamWriter Writer { get; }
    internal StreamReader Reader { get; }

    TcpClient client;
    ServerObject server;

    internal ClientObject(TcpClient tcpClient, ServerObject serverObject)
    {
        client = tcpClient;
        server = serverObject;
        var stream = client.GetStream();
        Reader = new StreamReader(stream);
        Writer = new StreamWriter(stream);
    }

    internal async Task ProcessAsync()
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
                case "Test":
                    TestAction();
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
        Game game = new Game(element.GetProperty("Name").ToString());
        server.AllPendingGames.Add(game);
        Dictionary<string, object> reply = new()
        {
            { "Type", "Create" },
            { "Status", "Done" },
            { "Game", game}
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
        string message = JsonSerializer.Serialize(server.AllPendingGames);

        Writer.Write(message);
        Writer.Flush();

        Console.WriteLine($"{Id}:message sent");
    }

    private void GameAction(JsonElement element)
    {
    }

    private void TestAction()
    {
        Dictionary<string, string> reply = new()
        {
            { "Test", "Ok" }
        };
        string message = JsonSerializer.Serialize(reply);
        
        Writer.Write(message);
        Writer.Flush();

        Console.WriteLine($"{Id}:message sent");
    }


    internal void Close()
    {
        Console.WriteLine($"{Id}:connection closed");
        Writer.Close();
        Reader.Close();
        client.Close();
    }
}