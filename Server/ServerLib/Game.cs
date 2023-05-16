namespace ServerLib;

class Game
{
    public string Name { get; }
    public string Id { get; }

    public int Count { get; set; }
    
    internal ClientObject Host { get; set; }
    internal ClientObject Join { get; set; }
    
    internal Game(string name)
    {
        Name = name;
        Id = Guid.NewGuid().ToString();
    }
}