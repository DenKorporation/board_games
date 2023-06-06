using System.Text.Json;

namespace ServerLib;

class Game
{
    public string Name { get; }
    public string Id { get; }

    internal const int MaxCount = 2;
    public int Count { get; set; }

    internal bool Started { get; set; }

    private object lockObj;

    internal ServerObject Server { get; set; }
    internal ClientObject? Host { get; set; }
    internal ClientObject? Join { get; set; }

    internal bool AddPlayer(ClientObject player)
    {
        if (Count == 0)
        {
            Host = player;
            Count++;
        }
        else if (Count < MaxCount)
        {
            Join = player;
            Count++;
            if (Count == MaxCount)
            {
                lock (Server.AllPendingGames)
                {
                    Server.AllPendingGames.Remove(this);
                }
                lock (Server.AllActiveGames)
                {
                    Server.AllActiveGames.Add(this);
                }
                Started = true;
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    internal bool RemovePlayer(ClientObject player)
    {
        if (Host == player)
        {
            Host = Join;
            Join = null;
            Count--;
        }
        else if (Join == player)
        {
            Join = null;
            Count--;
        }
        else
        {
            return false;
        }

        Host?.SendDisconnectEvent();

        return true;
    }

    internal void Start()
    {
        Host?.SendGameStartEvent();
        Join?.SendGameStartEvent();
    }

    internal void SendAction(JsonElement message, ClientObject client)
    {
        if (client == Host)
        {
            Join?.SendAction(message);
        }
        else
        {
            Host?.SendAction(message);
        }
    }

    internal Game(string name)
    {
        Name = name;
        Id = Guid.NewGuid().ToString();
    }
}