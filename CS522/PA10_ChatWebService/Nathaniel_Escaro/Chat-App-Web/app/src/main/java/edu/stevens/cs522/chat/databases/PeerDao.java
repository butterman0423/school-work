package edu.stevens.cs522.chat.databases;

import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;
import androidx.room.Transaction;
import androidx.room.Update;

import com.google.common.util.concurrent.ListenableFuture;

import java.util.List;

import edu.stevens.cs522.chat.entities.Peer;

/*
 * No insertions should be done on the main thread anymore!
 */
@Dao
public abstract class PeerDao {

    private static final String TAG = PeerDao.class.getCanonicalName();

    /**
     * Get all peers in the database.
     */
    @Query("SELECT * FROM peer")
    public abstract LiveData<List<Peer>> fetchAllPeers();

    /**
     * Get a single peer record (may be used in later assignments)
     */
    @Query("SELECT * FROM peer WHERE id=:peerId")
    public abstract ListenableFuture<Peer> fetchPeer(long peerId);

    /**
     * Get the database primary key for a peer, based on chat name.
     */
    @Query("SELECT id FROM peer WHERE name LIKE :name")
    protected abstract long getPeerId(String name);

    /**
     *  Insert a peer and return their primary key (must not already be in database)
     */
    @Insert(onConflict = OnConflictStrategy.IGNORE)
    public abstract long insert(Peer peer);

    /**
     * Update the metadata for a peer (GPS coordinates, last seen)
     */
    @Update
    protected abstract void update(Peer peer);

    @Transaction
    /**
     * update information if it is already defined.
     * This operation must be transactional, to avoid race condition
     * between search and insert
     */
    public long upsert(Peer peer) {
        long id = this.getPeerId(peer.name);
        if(id == 0) {
            insert(peer);
            return 0;
        }
        update(peer);
        return 0;
    }
}
