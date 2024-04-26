package edu.stevens.cs522.chat.databases;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;
import androidx.room.Transaction;
import androidx.room.Update;

import java.util.List;

import edu.stevens.cs522.chat.entities.Peer;

/*
 *
 * We will continue to allow insertion to be done on main thread for noew.
 */
@Dao
public abstract class PeerDao {

    /**
     * Get all peers in the database.
     * @return
     */
    @Query("SELECT * FROM Peer")
    public abstract LiveData<List<Peer>> fetchAllPeers();

    /**
     * Get the database primary key for a peer, based on chat name.
     * @param name
     * @return
     */
    @Query("SELECT id FROM Peer WHERE name LIKE :name")
    protected abstract long getPeerId(String name);

    /**
     *  Insert a peer and return their primary key (must not already be in database)
     * @param peer
     * @return
     */
    @Insert(onConflict = OnConflictStrategy.IGNORE)
    public abstract long insert(Peer peer);

    /**
     * Update the metadata for a peer (GPS coordinates, last seen)
     * @param peer
     */
    @Update
    protected abstract void update(Peer peer);

    @Transaction
    /**
     * update information if it is already defined.
     * This operation must be transactional, to avoid race condition
     * between search and insert
     */
    public void upsert(Peer peer) {
        long id = this.getPeerId(peer.name);
        if(id == 0) {
            insert(peer);
            return;
        }
        update(peer);
    }
}
