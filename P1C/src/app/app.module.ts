import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import { RouterModule } from '@angular/router';
import { MaterialModule } from '@angular/material';

import { AppComponent } from './app.component';
import { AddPeopleComponent } from './components/add-people/add-people.component';
import { AddMovieComponent } from './components/add-movie/add-movie.component';
import { AddAmRelationComponent } from './components/add-am-relation/add-am-relation.component';
import { AddDmRelationComponent } from './components/add-dm-relation/add-dm-relation.component';
import { SearchComponent } from './components/search/search.component';
import { AboutComponent } from './components/about/about.component';
import { ActorComponent } from './components/actor/actor.component';
import { MovieComponent } from './components/movie/movie.component';

@NgModule({
  declarations: [
    AppComponent,
    AddPeopleComponent,
    AddMovieComponent,
    AddAmRelationComponent,
    AddDmRelationComponent,
    SearchComponent,
    AboutComponent,
    ActorComponent,
    MovieComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    MaterialModule.forRoot(),
    RouterModule.forRoot([
      { path: '', component: SearchComponent},
      { path: 'add-people', component: AddPeopleComponent},
      { path: 'add-movie', component: AddMovieComponent},
      { path: 'add-am-relation', component: AddAmRelationComponent},
      { path: 'add-dm-relation', component: AddDmRelationComponent},
      { path: 'actor/:id', component: ActorComponent},
      { path: 'movie/:id', component: MovieComponent},
      { path: 'search', component: SearchComponent},
      { path: 'about', component: AboutComponent}
    ]),
    HttpModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
