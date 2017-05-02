# ColouringBook - Advanced Game Programming - Alejandro Garzón

## Role: C++ Multiplayer Programmer 

I was mainly responsible for adding local and online multiplayer to the game. This responsability allowed me not only to understand Unreal´s networking layer for multiplayer games, but also to code exclusively in C++. The latter was important for me because I wanted to go one step beyond from using just Blueprint.

## Understanding Unreal Engine Networking 

The engine has a networking layer based on a client-server pattern that implements all connection/interpolation/replication code. Therefore, we just mainly need to worry about deciding the code that is going to be executed in the server and in the client by using the proper network ownership/role, along with the objects that are going to be replicated.

### Replication

In order to share information between all the clients and the server, we must specify which objects are going to be replicated by setting the bReplicated flag. A replicated object can be anyone that inherits from the class AActor and additionaly there is the option via UPPROPERTY() to set a member of the class to also be replicated. It is up to the engine in the net update to take care of replicated objects and update all network related things. 

There are two main classes that are used to share information between different players: PlayerState(for player specific information like score, etc.) and GameState(for game specific information like connected players, etc.). Those two classes can be inherited to create player and game specific information to be replicated.

### Network ownership/roles

Any replicated object can have 3 types of roles or network ownership:

- Role_Authority: It is the version of the object that actually owns the server and therefore, the only one that can modify the state of that object.
- Role_AutonomousProxy: It is the version of the object that actually exist on the owning client and has some control over it, mainly receive and process player input.
- Role_SimulatedProxy: It is the version of the object that actually exist in the rest of the clients and do not have any control over the object or rights to change each state.

### Remote Procedure Calls(RPC)

RPCs are functions that are called by the local client or the server, but are executed remotely in the client or the server depending on the type of RPC. There are 3 types of RPCs:

- Client: It is called by the server and executed on the owning client, in other words, the one who has an ownership of Role_AutonomousProxy.
- Server: It is called by the owning client and executed by the server.
- NetMultiCast: It is called by the server and executed by all the clients regardless of the ownership.

Additionaly, this RPCs can be define with the property Reliable/unreliable so we can define which one must guarantee to be executed or ignore in case there are bandwith stress conditions.

### Travelling

Online multiplayer games use Uworld::ServerTravel to get the server to the loaded map along with the connected clients. Travelling can be done seamless, meaning that the clients will not disconnect and connect again when the travelling happens(as opposed to non-seamless), resulting in a better experience.

### Server types

The engine allows to set either a dedicated server or a listen server. The first one doesn´t own a player character and basically handles client connections and world update, whereas the second one has a player character and acts as a client and as a host too.

A dedicated server is best suited for large number of players and complex world updates. The listen server is best suited for relative small amount of connections, after all, it must handle processing time for the rest of the clients, but in contrast the client acting as a host has the benefit of having no latency.

## Main contributions

### Local Multiplayer

For the initial prototype, the idea was to have multiple players playing in the same machine. The key part here was to create the desired number of players and attach them to the correct spawn point. Assigning a player to the correct spawn point was important, because otherwise it could be spawned in a random point, that potentially could collide with another player, resulting the player not spanwing at all because of the collision. It took some time to figure out why a player wasn´t sometimes spawned and how to fix it. The solution was based on overriding GameMode::ChoosePlayerStart so we could assign a player to each fixed spawn point.

Below the commits, from most recent to oldest.
Branch: https://github.com/parmandorc/ColouringBook/tree/master

| commit |  summary  | 
|-------- |----------|
|<a href="https://github.com/parmandorc/ColouringBook/commit/f706af9dabb64a56cee448ccd929b0d4ac52bc95">f706af9</a> | We really need to include Engine.h if we want to get access to PlayerStart |
|<a href="https://github.com/parmandorc/ColouringBook/commit/1103ecf95763a029697d72fedbaa012fe12dd9f7">1103ecf</a> | PR´s feedback: Set num players to 2 instead of 4, include EngineMinimal.h | 
| <a href="https://github.com/parmandorc/ColouringBook/commit/6bb3041bd7da52b7ed2ce9a1ba7f96c36a076906">6bb3041</a> | Autoposses player and AutoreceiveInput according to PlayerController |
|<a href="https://github.com/parmandorc/ColouringBook/commit/36a482f56c9348cf838064116f863a27071448bb">36a482f</a> | Ignore enc_temp_folder |
|<a href="https://github.com/parmandorc/ColouringBook/commit/f3133cdd72bae8d1bb2d4af96dcba3b7872d665e">f3133cd</a> | Make sure that each player controller spawns the pawn in the correct PlayerStart |
|<a href="https://github.com/parmandorc/ColouringBook/commit/af449264d726f926ed660ccf515eee1d71da137d">af44926</a> | Use parent class GetNumPlayers for checking the numbers of players created |
|<a href="https://github.com/parmandorc/ColouringBook/commit/6ead55714af4c7e8af9894566601ecea96e4dbb6">6ead557</a> | Print properly number of created players debug message |
|<a href="https://github.com/parmandorc/ColouringBook/commit/1e47e984ce3675301d4d8349bce77c3f14d0388f">1e47398</a> | Multiple player creation |
|<a href="https://github.com/parmandorc/ColouringBook/commit/edce9831866e70a2f0dc4ae8959c125930747b4b">edce983</a> | Add to the map more PlayerStart with the purpose to spawn more than one players |
|<a href="https://github.com/parmandorc/ColouringBook/commit/f2f004d007eb403a50790fb34a5a96c17ecef5c2">f2f004d</a> | Do not split screen in local multiplayer |


### Online Multiplayer

When it came to add online multiplayer, first there was an investigation about how to do it. After the research, the easiest solution was to go ahead with a listen server. Also, it was quite clear the code that should be executed by the server and the client by using proper RPCs and network ownership roles. For example, we decided that the server would be the one responsible for spawning objects. As an example, the pseudocode below shows the change made to the local multiplayer version when a player fired:

```c++
void AColouringBookCharacter::FireShot(FVector FireDirection)
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
			if (isLocalMultiplayer)
			{
				fired = LocalFireShot(SpawnLocation, FireRotation);
			}
			else
			{
				fired = OnlineFireShot(SpawnLocation, FireRotation);
			}
	}
}

bool AColouringBookCharacter::LocalFireShot(FVector fireLocation, FRotator fireRotator)
{
  // Spawn Projectile
  // Play Sound
	return true;
}

bool AColouringBookCharacter::OnlineFireShot(FVector fireLocation, FRotator fireRotator)
{
	if (Role == ROLE_SimulatedProxy)
	{
		// Do not allow other simulated characters to perform this action
		return false;
	}
	// any visual firing effect
  
	// Notify the server
	ServerFireShot(netPlayerId, fireLocation, fireRotator);

	return true;
}

bool AColouringBookCharacter::ServerFireShot_Validate(int32 playerId, FVector fireLocation, FRotator fireRotator)
{	
	// Validation, anti-cheating measures, etc.
	return isValid;
}

void AColouringBookCharacter::ServerFireShot_Implementation(int32 playerId, FVector fireLocation, FRotator fireRotator)
{
	// Spawn projectil

	// Notify all clients that a player fired
	MulticastPlayerFired();
}

void AColouringBookCharacter::MulticastPlayerFired_Implementation()
{
	// Play Sound
}
```

Basically, with a server RPC we were notifying to the server that a player fired. The server was mainly spawning the projectile (if the notification succeeded the validation) and multicasting to all the players that the a player fired.

One of the issues that was happening is that the clients weren´t using the same shared fixed camera. At the begining, we thought that it could be because the camera wasn´t replicated at all, but after some investigation, it was mainly because each client´s PlayerController was managing the active camera. By default, the active camera was the one attached to the pawn that the PlayerController was owning, what makes sense for a FPS game but not for our game. Therefore, the solution was on creating a custom PlayerController in order to not use that specific property. 

In order to debug and identify the server and the clients, a very basic network lobby was implemented. In order to achieve this, a new map was created and some UI work was done too. From a functionality point of view, clients just connected to the server were waiting to the server for travelling to the battle map. 

Finally, one of the most challenging issue was how to replicate the ink of each player that was on the canvas to all the clients. Taking into account that we couldn´t replicate the entire texture for performance reasons, we ended up using RPCs for this purpose. Basically, the server multicasts the color, position and radius of the circle to be drawn, so each client can paint the canvas at the correct spot. Although this solution works, performance wise was a bit slow because we were doing a multicast for each drop that was colliding with the canvas. Several solutions were discussed with the team. The first one was about controlling the multicast frequency in order to mitigate bandwith penalty due to multicast more than one ink drop collision in a short period of time. The second one was about multicasting an event to spawn the ink drops with the correct colour and let the client  do all the collisions between the drop and the canvas, so each client could end up with an slightly different painted canvas, but with a number of multicast reduced considerably. We decided to go ahead with the latter one but there was no success, because the clients couldn´t see the ink on the canvas, only the server. This could be because some of the canvas drawing algorithm variables will probably need to be replicated, but further investigation is needed to detect the problem. 

Below the commits, from most recent to oldest.
Branch: https://github.com/parmandorc/ColouringBook/tree/online-multiplayer

| commit |  summary  | 
|---------- |------------|
| <a href="https://github.com/parmandorc/ColouringBook/commit/33cf2d4dc513260f005fd107809960de026ecdbc">33cf2d4</a> | Change DiffuseInk and ColorPixel to use gamestate instead of gamemode. Get the color by player index too |
| <a href="https://github.com/parmandorc/ColouringBook/commit/321966a2b66cf2562e6fe9ca5f001105833123eb">321966a</a> | Multicast that an enemy fired, so each client can play a fire sound | 
| <a href="https://github.com/parmandorc/ColouringBook/commit/890eb10d7e51db7d10689e7f5a8445387620cc58">890eb10</a> | Remove not needed code from the projectile |
| <a href="https://github.com/parmandorc/ColouringBook/commit/dc8cb762c1ec6f2c93768ffa4f8a902cddd62066">dc8cb76</a> | More fixes after AI merge: make the color of each player/ink to be back |
| <a href="https://github.com/parmandorc/ColouringBook/commit/f13f402264324df5e1060566b891cd69afd3aad5">f13f402</a> | More fixes after AI merge |
| <a href="https://github.com/parmandorc/ColouringBook/commit/bea4e50df2e5ad0227e67965a9eca097d35f4a25">bea4e50</a> | Only let the server to proccess hits. Avoid null pointer crash too |
| <a href="https://github.com/parmandorc/ColouringBook/commit/bc2e92aaafcc18964d2fc04c68b4f53ea4309ae8">bc2e92a</a> | Merge with conflicts resolved |
| <a href="https://github.com/parmandorc/ColouringBook/commit/b7c83385c3d4cc43857f41ab88dd9cb26676f85b">b7c8338</a> | Only allow the server to spawn puppets |
| <a href="https://github.com/parmandorc/ColouringBook/commit/19dd69705aafcb871913a36888c0d31a62ac87e4">19dd697</a> | Make the clients to paint the canvas by multicasting the position, radius and color |
| <a href="https://github.com/parmandorc/ColouringBook/commit/d8d7e8786bf54899de447d25a8f1e7b7669cdb96">d8d7e87</a> | Let´s make the canvas work again at least in the server |
| <a href="https://github.com/parmandorc/ColouringBook/commit/e068c94ace16e6066f1354f082881186d830303e">e068c94</a> | Allow only the server to process hits on the canvas |
| <a href="https://github.com/parmandorc/ColouringBook/commit/e1b5f09272cbe0b5ed02949d34993d3b4b37eb8a">e1b5ff09</a> | Replicate the ownerID so we notify the client when the owner of a projectile and inkDrop changes |
| <a href="https://github.com/parmandorc/ColouringBook/commit/677458fe24009231def2fe82eff382306e068b52">677458f</a> | Let the server know WHO fires in order ti apply the correct ink color to the drop | 
| <a href="https://github.com/parmandorc/ColouringBook/commit/69dfa1466712c6f7368baeb9c5cc9e601e7432a2">69dfa14</a> | Replicate server state so each client is able to update the HUD accordingly |
| <a href="https://github.com/parmandorc/ColouringBook/commit/4a30376b7009c1c90f3f47d6174d6aa16440fd7c">4a30376</a> | Make sure wer reset the static state when nding the game from the editor|
| <a href="https://github.com/parmandorc/ColouringBook/commit/2d6fa64a95dfff2c534b593986f2436f207d8a03">2d6fa64</a> | Display more info like connected players, etc. in the NetworkLobbyHUD |
| <a href="https://github.com/parmandorc/ColouringBook/commit/3ad32a039adc5bf4bc44a49761d6f581927d212b">3ad32a0</a> | Remove Role checks because the GameMode always acts like Role_Authority |
| <a href="https://github.com/parmandorc/ColouringBook/commit/070d72ebceb3f7a78d2f93c66f49bb751b6290d3">070d72e</a> | Add basic Network lobby HUD and set it whenever we change the state |
| <a href="https://github.com/parmandorc/ColouringBook/commit/d97cf839fac70c84dbd1ead4a0628963e53cc5b7">d97cf83</a> | Network lobby: Add empty lobby map and different states to wait for travelling, etc. |
| <a href="https://github.com/parmandorc/ColouringBook/commit/a46390430c4b15402c9e3ebdcec9fded67e5bd8a">a463904</a> |  Avoid null pointer crash when spawning puppets |
| <a href="https://github.com/parmandorc/ColouringBook/commit/605e7c19bcf8f3fb45b93b6130638f975ce92fff">605e7c1</a> | Make sure that all the clients are actually using the server camera  |
| <a href="https://github.com/parmandorc/ColouringBook/commit/a77c07d9f05cb7a895e686370b96570bedde465d">a77c07d</a> | Get rid of not needed functions that were intended for player spawning in local multiplayer |
| <a href="https://github.com/parmandorc/ColouringBook/commit/4ce7e416b585023f56f92194316cd1d3c8dd2678">4ce7e41</a> | Fix the issue when the client was disconnected from the server when validing a fire shot  | 
| <a href="https://github.com/parmandorc/ColouringBook/commit/ece7ffdc747e9f36cd1705234a6e580c702d42a7">ece7ffd</a> | Make sure that the inkDrop and the Canvas replicates |
| <a href="https://github.com/parmandorc/ColouringBook/commit/dba8fe837937f7b74d1111e5d954b7863cc51669">dba8fe8</a> | Spawn new players to the correct PlayerStart after login|
| <a href="https://github.com/parmandorc/ColouringBook/commit/c80d3cb1de3fd1aa997cc27194080949a002774e">c80d3cb</a> | Merge with conflicts resolved |
| <a href="https://github.com/parmandorc/ColouringBook/commit/3fc81c60231ef3762aa7659d9dc96662bd622b8b">3fc81c6</a> | Make the server to spawn projectiles |
| <a href="https://github.com/parmandorc/ColouringBook/commit/9fac34603a8d477f749a53bba8a806a2b918dc85">9fac346</a> | Skeleton added for online multiplayer firing action with basic RPC |
| <a href="https://github.com/parmandorc/ColouringBook/commit/9966e934c9704c217b30e27e72faa70169ab2fd7">9966e93</a> | Separate FireShot into Local and Online according to multiplayer mode |
| <a href="https://github.com/parmandorc/ColouringBook/commit/1f7275462917bab24a939968c271001cd9d04513">1f72754</a> | Only AutoPossessPlayer and AutoReceiveInput in local multiplayer |
| <a href="https://github.com/parmandorc/ColouringBook/commit/17498c11ac8560aed52ce6990ce880b78541296a">17498c1</a> | Separate local multiplayer and online multiplayer |

Below the commits, from most recent to oldest.
Branch: https://github.com/parmandorc/ColouringBook/tree/online-multiplayer-MulticastSpawnInkDrops

| commit |  summary  | 
|---------- |------------|
| <a href="https://github.com/parmandorc/ColouringBook/commit/c08637701445d9efd62f2afb86b99ddc8ae83c61">c086377</a> | Attempt to multicast spawn ink drops event in order to optimise canvas drawing |


## Team

After having a couple of short meetings with artist about the game design and mechanics, it was clear the type of game we needed to develop so everybody was on the same page. Pablo took the role of Lead Programmer and taking into account each programmer desires about what to do and each personal circumstances(part time students, full time workers, etc.), in my opinion it was a good idea about splitting and assigning work by disciplines (gameplay/AI, physics, multiplayer/networking). This allowed to each programmer to focus on a topic they were really interested and/or assessed, while at the same time reducing some dependencies or communication with the artists, like in my case. Indeed, Pablo was my main point of contact for any programming issue and I was mainly shielded from any mechanic or minor design change in the game, due to the fact that my job was mainly in the back-end by adding multiplayer to any main feature of the game.

However, due to the fact that everyone in the team was new to networking games with Unreal, we suffered from implementing without having online multiplayer in mind. The most clear example is that we were using a custom id to identify a player when probably we shouldn´t, as Unreal already does it for you. I personally change this but with any new merge that was coming to the multiplayer branch, many conflicts arose that needed to be resolved and in some cases ended up loosing previous online multiplayer work and do some unexpected work to get the online multiplayer back again. This was causing a severe problem and taking into account that we were close to deadline, still some gameplay work to be finished(without multiplayer in mind) and the performance issue we were having to replicate the painted canvas, we agreed not to merge the online multiplayer branch into master for the deadline, so at least we could offer a local multiplayaer demo with all the features.

## Conclusions and future work

Probably the most important learning that I get from this project is that you have to develop your game with online multiplayer in mind from the very beginning, regardless if it ends up being multiplayer or not, mainly because it can save a lot of refactoring time.

Therefore, I would say that the main thing to do going forward would be to architect current game from a multiplayer point of view and fix properly the performance issue we have with replicating the ink on the canvas. 
