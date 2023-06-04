using System.Net.Sockets;
using System.Text.Json;

namespace ServerLib;

class ClientObject
{
    internal string Id { get; } = Guid.NewGuid().ToString();
    internal StreamWriter Writer { get; }
    internal StreamReader Reader { get; }

    internal Game? Game { get; set; }

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
            Console.WriteLine($"{Id}: Connection opened");

            string? message = await Reader.ReadLineAsync();
            using JsonDocument doc = JsonDocument.Parse(message);
            JsonElement element = doc.RootElement;

            switch (element.GetProperty("Type").ToString())
            {
                case "Create":
                    CreateRoomAction(element);
                    break;
                case "Connect":
                    await ConnectRoomAction(element);
                    break;
                case "List":
                    ListRoomAction();
                    break;
                case "Game":
                    GameAction(element);
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
        Game = new Game(element.GetProperty("Name").ToString());
        Game.Server = server;
        lock (server.AllPendingGames)
        {
            server.AllPendingGames.Add(Game);   
        }
        Dictionary<string, object> reply = new()
        {
            { "Type", "Create" },
            { "Status", "Done" },
            { "Game", Game }
        };

        Writer.WriteLine(JsonSerializer.Serialize(reply));
        Writer.Flush();

        Console.WriteLine($"{Id}: the room has been created");
    }

    private async Task ConnectRoomAction(JsonElement element)
    {
        lock (server.AllPendingGames)
        {
            Game = server.AllPendingGames.FirstOrDefault(game => game.Id == element.GetProperty("Id").ToString());
        }
        Dictionary<string, object> reply;
        if (Game == null || !Game.AddPlayer(this))
        {
            reply = new()
            {
                { "Type", "Connect" },
                { "Status", 404 }
            };
            Writer.WriteLine(JsonSerializer.Serialize(reply));
            Writer.Flush();

            Console.WriteLine($"{Id}: room not found");
        }
        else
        {
            reply = new()
            {
                { "Type", "Connect" },
                { "Status", 200 },
                { "Game", Game }
            };
            Writer.WriteLine(JsonSerializer.Serialize(reply));
            Writer.Flush();

            Console.WriteLine($"{Id}: joined the room {Game.Id}");
            if (Game.Started)
            {
                Game.Start();
            }

            try
            {
                while (true)
                {
                    string? message = await Reader.ReadLineAsync();
                    if (!String.IsNullOrEmpty(message))
                    {
                        using JsonDocument doc = JsonDocument.Parse(message);
                        if (doc.RootElement.GetProperty("Type").ToString() == "Disconnect")
                        {
                            break;
                        } else if (doc.RootElement.GetProperty("Type").ToString() == "Game")
                        {
                            break;
                        }
                    }
                }
                Console.WriteLine($"{Id}: room {Game.Id}: leave room");
                
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
            finally
            {
                DisconnectAction();
            }
        }
    }

    private void DisconnectAction()
    {
        Game?.RemovePlayer(this);
        Console.WriteLine($"{Id}: leaved the room {Game.Id}");
    }

    internal void SendDisconnectEvent()
    {
        Dictionary<string, object> reply = new()
        {
            { "Type", "Disconnect" },
            { "Game", Game }
        };
        Writer.WriteLine(JsonSerializer.Serialize(reply));
        Writer.Flush();

        Console.WriteLine($"{Id}: room {Game.Id}: disconnect event sent");
    }

    internal void SendGameStartEvent()
    {
        Dictionary<string, object> reply = new()
        {
            { "Type", "Game" },
            { "Status", "Start" },
            { "Game", Game }
        };

        Writer.WriteLine(JsonSerializer.Serialize(reply));
        Writer.Flush();

        Console.WriteLine($"{Id}: room {Game.Id}: game start event sent");
    }

    private void ListRoomAction()
    {
        string message;
        lock (server.AllPendingGames)
        {
            message = JsonSerializer.Serialize(server.AllPendingGames);   
        }

        Writer.WriteLine(message);
        Writer.Flush();

        Console.WriteLine($"{Id}: list of games sent");
    }

    private void GameAction(JsonElement element)
    {
    }

    private void TestAction()
    {
        Dictionary<string, string> reply = new()
        {
            { "Type", "Test" },
            { "Status", "Ok" }
        };
        string message = JsonSerializer.Serialize(reply);

        Writer.WriteLine(message);
        Writer.Flush();

        Console.WriteLine($"{Id}: test message sent");
    }


    internal void Close()
    {
        Console.WriteLine($"{Id}: connection closed");
        Writer.Close();
        Reader.Close();
        client.Close();
    }
}