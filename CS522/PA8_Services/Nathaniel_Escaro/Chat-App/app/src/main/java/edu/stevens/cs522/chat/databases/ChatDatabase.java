package edu.stevens.cs522.chat.databases;

import android.content.Context;

import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;
import androidx.room.TypeConverters;

import edu.stevens.cs522.chat.entities.Chatroom;
import edu.stevens.cs522.chat.entities.DateConverter;
import edu.stevens.cs522.chat.entities.Message;
import edu.stevens.cs522.chat.entities.Peer;

/**
 * Created by dduggan.
 *
 * See build.gradle file for app for where schema file is left after processing.
 */

@Database(entities = { Chatroom.class, Message.class, Peer.class }, version = 1)
public abstract class ChatDatabase extends RoomDatabase {

    private static final String DATABASE_NAME = "messages.db";

    private static ChatDatabase instance;

    @TypeConverters(DateConverter.class)
    public abstract PeerDao peerDao();

    @TypeConverters(DateConverter.class)
    public abstract ChatroomDao chatroomDao();

    @TypeConverters(DateConverter.class)
    public abstract MessageDao messageDao();

    public static ChatDatabase getInstance(Context context) {
        if (instance == null) {
            instance = Room.databaseBuilder(context, ChatDatabase.class, DATABASE_NAME).build();
        }
        return instance;
    }

}