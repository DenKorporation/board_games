namespace ServerLib;

public class Game
{
    public string Name { get; }
    public string Id { get; }

    internal ClientObject Host { get; set; }
    internal ClientObject Client { get; set; }
    
    public Game(string name)
    {
        Name = name;
        Id = Guid.NewGuid().ToString();
    }
}