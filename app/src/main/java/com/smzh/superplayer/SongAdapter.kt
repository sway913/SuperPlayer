package com.smzh.superplayer

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.smzh.superplayer.sing.Song
import kotlinx.android.synthetic.main.layout_song_item.view.*

class SongAdapter(private val listener: ChooseSongListener) : RecyclerView.Adapter<SongAdapter.SongHolder>() {

    private val songList = arrayListOf<Song>()

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): SongHolder {
        return SongHolder(LayoutInflater.from(parent.context).inflate(R.layout.layout_song_item, parent, false))
    }

    override fun onBindViewHolder(holder: SongHolder, position: Int) {
        val song = songList[position]
        holder.onBind(song)
    }


    override fun getItemCount(): Int {
        return songList.size
    }

    fun refresh(songs: List<Song>) {
        songList.clear()
        songList.addAll(songs)
        notifyDataSetChanged()
    }


    inner class SongHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {

        fun onBind(song: Song){
            itemView.run {
                btn_sing.setOnClickListener{
                    listener.onSongChosen(song)
                }
                song_name.text = song.name
                singer.text = song.singer
            }
        }
    }

    interface ChooseSongListener {
        fun onSongChosen(song: Song);
    }


}