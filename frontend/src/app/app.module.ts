import { NgModule } from '@angular/core';
import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { BrowserModule } from '@angular/platform-browser';
import { ListComponent } from "../lib/feature/list/list.component";
import { ItemComponent } from "../lib/feature/item/item.component";
import { NavbarComponent } from "../lib/feature/navbar/navbar.component";
import { ReminderAppComponent } from "../lib/feature/reminderApp/reminderApp.component";
import { FilterButtonComponent } from "../lib/ui/filterButton/filterButton.component";
import { CreateButtonComponent } from "../lib/ui/createButton/createButton.component";
import { ListObjectComponent } from "../lib/ui/listObject/listObject.component";
import { TrashComponent } from "../lib/ui/trash/trash.component";
import { HttpClientModule } from '@angular/common/http';

@NgModule({
  declarations: [AppComponent, ReminderAppComponent, ListComponent, ItemComponent, NavbarComponent, FilterButtonComponent,
    CreateButtonComponent, ListObjectComponent, TrashComponent],
  imports: [BrowserModule, AppRoutingModule, HttpClientModule],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
