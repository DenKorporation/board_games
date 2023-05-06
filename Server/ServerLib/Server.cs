using System.Net.Sockets;

namespace ServerLib;

public struct Game
{
    public string Name;
    public string Id;

    public Game(string name)
    {
        Name = name;
        Id = Guid.NewGuid().ToString();
    }
}

public class ServerObject
{
    public TcpListener tcpListener;
    List<ClientObject> clients = new(); // все подключения

    public List<Game> AllGame { get; } = new ();


    public ServerObject()
    {
        for (int i = 0; i < 20; i++)
        {
            AllGame.Add(new Game("abracadabra"));
        }
    }

    protected internal void RemoveConnection(string id)
    {
        ClientObject? client = clients.FirstOrDefault(c => c.Id == id);
        if (client != null) clients.Remove(client);
        client?.Close();
    }
    public async Task ListenAsync()
    {
        try
        {
            while (true)
            {
                TcpClient tcpClient = await tcpListener.AcceptTcpClientAsync();
 
                ClientObject clientObject = new ClientObject(tcpClient, this);
                clients.Add(clientObject);
                Task.Run(clientObject.ProcessAsync);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex.Message);
        }
        finally
        {
            Disconnect();
        }
    }
    
    protected internal async Task BroadcastMessageAsync(string message, string id)
    {
        foreach (var client in  clients)
        {
            if (client.Id != id)
            {
                await client.Writer.WriteLineAsync(message);
                await client.Writer.FlushAsync();
            }
        }
    }
    
    protected internal void Disconnect()
    {
        foreach (var client in clients)
        {
            client.Close();
        }
        tcpListener.Stop();
    }
}