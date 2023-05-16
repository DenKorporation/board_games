﻿using System.Net.Sockets;
using System.Text;
using System.Text.Json;

namespace ServerLib;

public class ServerObject
{
    public TcpListener tcpListener;
    private List<ClientObject> clients = new(); // все подключения
    internal List<Game> AllPendingGames { get; } = new();
    internal List<Game> AllActiveGames { get; } = new();

    internal void RemoveConnection(string id)
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

    internal void Disconnect()
    {
        foreach (var client in clients)
        {
            client.Close();
        }

        tcpListener.Stop();
    }
}